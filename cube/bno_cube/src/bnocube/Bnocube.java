/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 *
 * http://www.java2s.com/example/java-api/javax/media/j3d/orientedshape3d/rotate_about_point-0.html
 * send an ascii string of CSV data values via a tcp port
 */
package bnocube;

import java.awt.*;
import java.util.Scanner;
import javax.media.j3d.BranchGroup;
import javax.media.j3d.Canvas3D;
import javax.media.j3d.Transform3D;
import javax.media.j3d.TransformGroup;
import javax.swing.JFrame;
import javax.vecmath.Quat4d;
import javax.vecmath.Vector3d;
import javax.vecmath.Point3f;
import com.fazecast.jSerialComm.SerialPort;
import com.sun.j3d.utils.geometry.ColorCube;
import javax.media.j3d.Text3D;
import com.sun.j3d.utils.universe.SimpleUniverse;
import java.util.Properties;
import java.net.Socket;
import java.net.InetAddress;
import javax.media.j3d.*;

/**
 *
 * @author root
 */
public class Bnocube {

    public static void main(String[] args) {

        String ttl_eth_host = "NA";
        JFrame frame = new JFrame("Sensor Fusion Visual Test Program");
        Canvas3D canvas = new Canvas3D(SimpleUniverse.getPreferredConfiguration());
        SimpleUniverse universe = new SimpleUniverse(canvas);
        BranchGroup group = new BranchGroup();
        ColorCube cube = new ColorCube(0.3);
        Font myFont = new Font("TimesRoman", Font.CENTER_BASELINE, 1);
        Font3D myFont3D = new Font3D(myFont, new FontExtrusion());
        Point3f textPt = new Point3f(-1.6f, 0.4f, 0.0f);
        Text3D myText3D = new Text3D(myFont3D, "BNO086", textPt);
        Shape3D myShape3D = new Shape3D(myText3D, new Appearance());

        TransformGroup transformGroup = new TransformGroup();
        transformGroup.setCapability(TransformGroup.ALLOW_TRANSFORM_WRITE);
        transformGroup.addChild(myShape3D);
        transformGroup.addChild(cube);

        universe.getViewingPlatform().setNominalViewingTransform();
        group.addChild(transformGroup);
        universe.addBranchGraph(group);

        frame.add(canvas);
        frame.setSize(800, 600);
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frame.setVisible(true);

        ttl_eth_host = "10.1.1.238";

        try {
            Socket socket = new Socket(ttl_eth_host, 20108);
            // send IP address query to remote module
            socket.getOutputStream().write("L\r".getBytes("US-ASCII")); // or UTF-8 or any other applicable encoding...

            // grab data from the IMU host tcp server
            Scanner s = new Scanner(socket.getInputStream());
            System.err.println("Scanner running.");

            while (s.hasNextLine()) {
                try {
                    String line = s.nextLine();
                    String[] token = line.split(",");

                    if (token[0].equals("  1")) {
                        System.out.println(String.format("dtype = %3s  device = %s :%s:%s:%s:%s:%s:%s:%s:", token[0], token[1], token[2], token[3], token[4], token[5], token[6], token[7], token[8]));
                        // multiply x/y/z by -1 to swap frames of reference
                        double x = Double.parseDouble(token[4]);
                        double y = Double.parseDouble(token[2]);
                        double z = -Double.parseDouble(token[3]);
                        double w = Double.parseDouble(token[5]);
                        double ax = Double.parseDouble(token[6]);
                        double ay = Double.parseDouble(token[7]);
                        double az = Double.parseDouble(token[8]);

                        Quat4d quaternion = new Quat4d(w, x, y, z);
                        Vector3d vector = new Vector3d((az * 0.02), (ay * 0.02), (az * 0.02));
                        transformGroup.setTransform(new Transform3D(quaternion, vector, 0.5));

                        // the inverse cosine of w gives you the pitch *if* you normalize the quaternion with x and z being zero
                        double pitch = Math.acos(w / Math.sqrt(w * w + y * y)) * 2.0 - (Math.PI / 2.0);

                        System.out.println(String.format("w = %+2.3f     x = %+2.3f     y = %+2.3f     z = %+2.3f     pitch = %+1.3f", w, x, y, z, pitch));
                    }
                } catch (Exception e) {
                    System.err.println("Scan Line error.");
                }
            }
            socket.close();
            s.close();
            System.err.println("Lost communication with the IMU socket. Exiting.");
            System.exit(1);
        } catch (Exception e) {
            System.err.println("Unable to open socket to ttl_eth_host. Exiting.");
            System.exit(1);
        }
        System.err.println("Exiting.");
        System.exit(1);
    }
}
