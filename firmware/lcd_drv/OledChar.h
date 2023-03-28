/************************************************************************/
/*																		*/
/*	OledChar.h	--	Interface Declarations for OledChar.c				*/
/*																		*/
/************************************************************************/
/*	Author:		Gene Apperson											*/
/*	Copyright 2011, Digilent Inc.										*/
/************************************************************************/
/*
  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */
/************************************************************************/
/*  File Description:													*/
/*																		*/
/*	Interface to OledChar.c												*/
/*																		*/
/************************************************************************/
/*  Revision History:													*/
/*																		*/
/*	06/01/2011(GeneA): created											*/
/*																		*/
/************************************************************************/

#if !defined(OLEDCHAR_H)
#define OLEDCHAR_H

#include "lcd_drv.h"

extern int32_t xcoOledCur;
extern int32_t ycoOledCur;

extern uint8_t * pbOledCur;
extern uint8_t mskOledCur;
extern int32_t bnOledCur;
extern int32_t fOledCharUpdate;

extern uint8_t rgbOledBmp[cbOledDispMax];

extern int32_t dxcoOledFontCur;
extern int32_t dycoOledFontCur;

extern uint8_t * pbOledFontCur;
extern uint8_t * pbOledFontUser;

void OledSetCursor(int32_t xch, int32_t ych);
void OledGetCursor(int32_t * pxcy, int32_t * pych);
int32_t OledDefUserChar(char ch, uint8_t * pbDef);
void OledSetCharUpdate(int32_t f);
int32_t OledGetCharUpdate();
void OledPutChar(char ch);
void OledPutString(char * sz);

/* ------------------------------------------------------------ */

#endif

/************************************************************************/
