/**
  @Generated Pin Manager Header File

  @Company:
    Microchip Technology Inc.

  @File Name:
    pin_manager.h

  @Summary:
    This is the Pin Manager file generated using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  @Description
    This header file provides APIs for driver for .
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.81.8
        Device            :  PIC18F14Q41
        Driver Version    :  2.11
    The generated drivers are tested against the following:
        Compiler          :  XC8 2.36 and above
        MPLAB 	          :  MPLAB X 6.00	
*/

/*
    (c) 2018 Microchip Technology Inc. and its subsidiaries. 
    
    Subject to your compliance with these terms, you may use Microchip software and any 
    derivatives exclusively with Microchip products. It is your responsibility to comply with third party 
    license terms applicable to your use of third party software (including open source software) that 
    may accompany Microchip software.
    
    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER 
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY 
    IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS 
    FOR A PARTICULAR PURPOSE.
    
    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP 
    HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO 
    THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL 
    CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT 
    OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS 
    SOFTWARE.
*/

#ifndef PIN_MANAGER_H
#define PIN_MANAGER_H

/**
  Section: Included Files
*/

#include <xc.h>

#define INPUT   1
#define OUTPUT  0

#define HIGH    1
#define LOW     0

#define ANALOG      1
#define DIGITAL     0

#define PULL_UP_ENABLED      1
#define PULL_UP_DISABLED     0

// get/set RB5 procedures
#define RB5_SetHigh()            do { LATBbits.LATB5 = 1; } while(0)
#define RB5_SetLow()             do { LATBbits.LATB5 = 0; } while(0)
#define RB5_Toggle()             do { LATBbits.LATB5 = ~LATBbits.LATB5; } while(0)
#define RB5_GetValue()              PORTBbits.RB5
#define RB5_SetDigitalInput()    do { TRISBbits.TRISB5 = 1; } while(0)
#define RB5_SetDigitalOutput()   do { TRISBbits.TRISB5 = 0; } while(0)
#define RB5_SetPullup()             do { WPUBbits.WPUB5 = 1; } while(0)
#define RB5_ResetPullup()           do { WPUBbits.WPUB5 = 0; } while(0)
#define RB5_SetAnalogMode()         do { ANSELBbits.ANSELB5 = 1; } while(0)
#define RB5_SetDigitalMode()        do { ANSELBbits.ANSELB5 = 0; } while(0)

// get/set RC1 procedures
#define RC1_SetHigh()            do { LATCbits.LATC1 = 1; } while(0)
#define RC1_SetLow()             do { LATCbits.LATC1 = 0; } while(0)
#define RC1_Toggle()             do { LATCbits.LATC1 = ~LATCbits.LATC1; } while(0)
#define RC1_GetValue()              PORTCbits.RC1
#define RC1_SetDigitalInput()    do { TRISCbits.TRISC1 = 1; } while(0)
#define RC1_SetDigitalOutput()   do { TRISCbits.TRISC1 = 0; } while(0)
#define RC1_SetPullup()             do { WPUCbits.WPUC1 = 1; } while(0)
#define RC1_ResetPullup()           do { WPUCbits.WPUC1 = 0; } while(0)
#define RC1_SetAnalogMode()         do { ANSELCbits.ANSELC1 = 1; } while(0)
#define RC1_SetDigitalMode()        do { ANSELCbits.ANSELC1 = 0; } while(0)

// get/set RC2 procedures
#define RC2_SetHigh()            do { LATCbits.LATC2 = 1; } while(0)
#define RC2_SetLow()             do { LATCbits.LATC2 = 0; } while(0)
#define RC2_Toggle()             do { LATCbits.LATC2 = ~LATCbits.LATC2; } while(0)
#define RC2_GetValue()              PORTCbits.RC2
#define RC2_SetDigitalInput()    do { TRISCbits.TRISC2 = 1; } while(0)
#define RC2_SetDigitalOutput()   do { TRISCbits.TRISC2 = 0; } while(0)
#define RC2_SetPullup()             do { WPUCbits.WPUC2 = 1; } while(0)
#define RC2_ResetPullup()           do { WPUCbits.WPUC2 = 0; } while(0)
#define RC2_SetAnalogMode()         do { ANSELCbits.ANSELC2 = 1; } while(0)
#define RC2_SetDigitalMode()        do { ANSELCbits.ANSELC2 = 0; } while(0)

// get/set MLED aliases
#define MLED_TRIS                 TRISCbits.TRISC3
#define MLED_LAT                  LATCbits.LATC3
#define MLED_PORT                 PORTCbits.RC3
#define MLED_WPU                  WPUCbits.WPUC3
#define MLED_OD                   ODCONCbits.ODCC3
#define MLED_ANS                  ANSELCbits.ANSELC3
#define MLED_SetHigh()            do { LATCbits.LATC3 = 1; } while(0)
#define MLED_SetLow()             do { LATCbits.LATC3 = 0; } while(0)
#define MLED_Toggle()             do { LATCbits.LATC3 = ~LATCbits.LATC3; } while(0)
#define MLED_GetValue()           PORTCbits.RC3
#define MLED_SetDigitalInput()    do { TRISCbits.TRISC3 = 1; } while(0)
#define MLED_SetDigitalOutput()   do { TRISCbits.TRISC3 = 0; } while(0)
#define MLED_SetPullup()          do { WPUCbits.WPUC3 = 1; } while(0)
#define MLED_ResetPullup()        do { WPUCbits.WPUC3 = 0; } while(0)
#define MLED_SetPushPull()        do { ODCONCbits.ODCC3 = 0; } while(0)
#define MLED_SetOpenDrain()       do { ODCONCbits.ODCC3 = 1; } while(0)
#define MLED_SetAnalogMode()      do { ANSELCbits.ANSELC3 = 1; } while(0)
#define MLED_SetDigitalMode()     do { ANSELCbits.ANSELC3 = 0; } while(0)

// get/set RLED aliases
#define RLED_TRIS                 TRISCbits.TRISC4
#define RLED_LAT                  LATCbits.LATC4
#define RLED_PORT                 PORTCbits.RC4
#define RLED_WPU                  WPUCbits.WPUC4
#define RLED_OD                   ODCONCbits.ODCC4
#define RLED_ANS                  ANSELCbits.ANSELC4
#define RLED_SetHigh()            do { LATCbits.LATC4 = 1; } while(0)
#define RLED_SetLow()             do { LATCbits.LATC4 = 0; } while(0)
#define RLED_Toggle()             do { LATCbits.LATC4 = ~LATCbits.LATC4; } while(0)
#define RLED_GetValue()           PORTCbits.RC4
#define RLED_SetDigitalInput()    do { TRISCbits.TRISC4 = 1; } while(0)
#define RLED_SetDigitalOutput()   do { TRISCbits.TRISC4 = 0; } while(0)
#define RLED_SetPullup()          do { WPUCbits.WPUC4 = 1; } while(0)
#define RLED_ResetPullup()        do { WPUCbits.WPUC4 = 0; } while(0)
#define RLED_SetPushPull()        do { ODCONCbits.ODCC4 = 0; } while(0)
#define RLED_SetOpenDrain()       do { ODCONCbits.ODCC4 = 1; } while(0)
#define RLED_SetAnalogMode()      do { ANSELCbits.ANSELC4 = 1; } while(0)
#define RLED_SetDigitalMode()     do { ANSELCbits.ANSELC4 = 0; } while(0)

// get/set RC6 procedures
#define RC6_SetHigh()            do { LATCbits.LATC6 = 1; } while(0)
#define RC6_SetLow()             do { LATCbits.LATC6 = 0; } while(0)
#define RC6_Toggle()             do { LATCbits.LATC6 = ~LATCbits.LATC6; } while(0)
#define RC6_GetValue()              PORTCbits.RC6
#define RC6_SetDigitalInput()    do { TRISCbits.TRISC6 = 1; } while(0)
#define RC6_SetDigitalOutput()   do { TRISCbits.TRISC6 = 0; } while(0)
#define RC6_SetPullup()             do { WPUCbits.WPUC6 = 1; } while(0)
#define RC6_ResetPullup()           do { WPUCbits.WPUC6 = 0; } while(0)
#define RC6_SetAnalogMode()         do { ANSELCbits.ANSELC6 = 1; } while(0)
#define RC6_SetDigitalMode()        do { ANSELCbits.ANSELC6 = 0; } while(0)

/**
   @Param
    none
   @Returns
    none
   @Description
    GPIO and peripheral I/O initialization
   @Example
    PIN_MANAGER_Initialize();
 */
void PIN_MANAGER_Initialize (void);




#endif // PIN_MANAGER_H
/**
 End of File
*/