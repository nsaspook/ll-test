/*******************************************************************************
  TMR Peripheral Library Interface Source File

  Company
    Microchip Technology Inc.

  File Name
    plib_tmr9.c

  Summary
    TMR9 peripheral library source file.

  Description
    This file implements the interface to the TMR peripheral library.  This
    library provides access to and control of the associated peripheral
    instance.

*******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
* Copyright (C) 2019 Microchip Technology Inc. and its subsidiaries.
*
* Subject to your compliance with these terms, you may use Microchip software
* and any derivatives exclusively with Microchip products. It is your
* responsibility to comply with third party license terms applicable to your
* use of third party software (including open source software) that may
* accompany Microchip software.
*
* THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
* EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
* WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
* PARTICULAR PURPOSE.
*
* IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
* INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
* WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
* BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
* FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
* ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
* THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
*******************************************************************************/
// DOM-IGNORE-END


// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include "device.h"
#include "plib_tmr9.h"



void TMR9_Initialize(void)
{
    /* Disable Timer */
    T9CONCLR = _T9CON_ON_MASK;

    /*
    SIDL = 0
    SYNC = 0
    TGATE = 0
    TCKPS =7
    T32   = 1
    TCS = 0
    */
    T9CONSET = 0x78;

    /* Clear counter */
    TMR9 = 0x0;

    /*Set period */
    PR9 = 3749999999U;


}


void TMR9_Start(void)
{
    T9CONSET = _T9CON_ON_MASK;
}


void TMR9_Stop (void)
{
    T9CONCLR = _T9CON_ON_MASK;
}

void TMR9_PeriodSet(uint32_t period)
{
    PR9  = period;
}

uint32_t TMR9_PeriodGet(void)
{
    return PR9;
}

uint32_t TMR9_CounterGet(void)
{
    return (TMR9);
}


uint32_t TMR9_FrequencyGet(void)
{
    return (234375);
}

