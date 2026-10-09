/*********************************************************************
 *
 *      Microchip USB C18 Firmware -  USB Bootloader Version 1.00
 *
 *********************************************************************
 * FileName:        boot.c
 * Dependencies:    See INCLUDES section below
 * Processor:       PIC18
 * Compiler:        C18 2.30.01+
 * Company:         Microchip Technology, Inc.
 *
 * Software License Agreement
 *
 * The software supplied herewith by Microchip Technology Incorporated
 * (the “Company”) for its PICmicro® Microcontroller is intended and
 * supplied to you, the Company’s customer, for use solely and
 * exclusively on Microchip PICmicro Microcontroller products. The
 * software is owned by the Company and/or its supplier, and is
 * protected under applicable copyright laws. All rights are reserved.
 * Any use in violation of the foregoing restrictions may subject the
 * user to criminal sanctions under applicable laws, as well as to
 * civil liability for the breach of the terms and conditions of this
 * license.
 *
 * THIS SOFTWARE IS PROVIDED IN AN “AS IS” CONDITION. NO WARRANTIES,
 * WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED
 * TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. THE COMPANY SHALL NOT,
 * IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL OR
 * CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 *
 * Author               Date        Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * Rawin Rojvanit       11/19/04    Original. USB Bootloader
 ********************************************************************/

/******************************************************************************
 * -boot.c-
 * This file contains functions necessary to carry out bootloading tasks.
 * The only 2 USB specific functions are BootInitEP() and BootService().
 * All other functions can be reused with other communication methods.
 *****************************************************************************/

/** I N C L U D E S **********************************************************/
#if defined(__XC8)
#include <xc.h>
#else
#include <p18cxxx.h>
#endif
#include "typedefs.h"
#include "usb.h"
#include "io_cfg.h"

/* Table instructions must remain explicit: ordinary C pointer accesses
 * do not provide the Flash programming semantics required here.
 */
#if defined(__XC8)
#define BootTableRead()  asm("tblrd*")
#define BootTableWrite() asm("tblwt*")
#else
#define BootTableRead()  _asm TBLRD* _endasm
#define BootTableWrite() _asm TBLWT* _endasm
#endif

/** V A R I A B L E S ********************************************************/
#if !defined(__XC8)
#pragma udata
#endif
byte counter;
byte trf_state;

word big_counter;

/** P R I V A T E  P R O T O T Y P E S ***************************************/
void BlinkUSBStatus(void);

/** D E C L A R A T I O N S **************************************************/
#if !defined(__XC8)
#pragma code
#endif

/** C L A S S  S P E C I F I C  R E Q ****************************************/

/** U S E R  A P I ***********************************************************/

/******************************************************************************
 * Function:        void BootInitEP(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        BootInitEP initializes bootloader endpoints, buffer
 *                  descriptors, internal state-machine, and variables.
 *                  It should be called after the USB host has sent out a
 *                  SET_CONFIGURATION request.
 *                  See USBStdSetCfgHandler() in usb9.c for examples.
 *
 * Note:            None
 *****************************************************************************/
void BootInitEP(void)
{   
    trf_state = WAIT_FOR_CMD;
    BOOT_UEP = EP_OUT_IN|HSHK_EN;               // Enable 2 data pipes

    /*
     * Do not have to init Cnt of IN pipes here.
     * Reason:  Number of bytes to send to the host
     *          varies from one transaction to
     *          another. Cnt should equal the exact
     *          number of bytes to transmit for
     *          a given IN transaction.
     *          This number of bytes will only
     *          be known right before the data is
     *          sent.
     */
    BOOT_BD_OUT.Cnt = sizeof(dataPacket);   // Set buffer size
    BOOT_BD_OUT.ADR = (byte*)&dataPacket;   // Set buffer address
    BOOT_BD_OUT.Stat._byte = _USIE|_DAT0|_DTSEN;// Set status

    BOOT_BD_IN.ADR = (byte*)&dataPacket;    // Set buffer address
    BOOT_BD_IN.Stat._byte = _UCPU|_DAT1;    // Set buffer status

}//end BootInitEP

/* offset is measured in bytes, including for erase commands. Loading
 * all three registers on each access preserves carries across 0xFFFF.
 */
static void LoadTablePointer(word offset)
{
    unsigned long address;
    address = (unsigned long)dataPacket.ADR.low |
              ((unsigned long)dataPacket.ADR.high << 8) |
              ((unsigned long)dataPacket.ADR.upper << 16);
    address += offset;
    TBLPTRU = (byte)(address >> 16);
    TBLPTRH = (byte)(address >> 8);
    TBLPTRL = (byte)address;
}

static void StartWrite(void)
{
    byte interrupt_enable;
    EECON1bits.WREN = 1;
    interrupt_enable = INTCON & 0xC0;
    INTCON &= 0x3F;       /* Disable both interrupt priority levels. */
#if defined(__XC8)
    /* PIC18F4550 access-bank SFRs: EECON2=0xFA7, EECON1=0xFA6.
     * Keep the unlock and WR set contiguous, independent of optimization.
     */
    asm("movlw 0x55\n"
        "movwf 0xFA7,0\n"
        "movlw 0xAA\n"
        "movwf 0xFA7,0\n"
        "bsf 0xFA6,1,0\n"
        "nop");
#else
    _asm
        MOVLW 0x55
        MOVWF EECON2, 0
        MOVLW 0xAA
        MOVWF EECON2, 0
        BSF EECON1, 1, 0
        NOP
    _endasm
#endif
    /* Flash stalls the CPU; EEPROM completes asynchronously. */
    while(EECON1bits.WR) { }
    EECON1bits.WREN = 0;
    INTCON |= interrupt_enable;
}

byte BootReadEEPROM(byte address)
{
    while(EECON1bits.WR) { }
    EECON1 = 0x00;        /* Data EEPROM, not Flash/configuration. */
    EEADR = address;
    EECON1bits.RD = 1;
    return EEDATA;
}

void BootWriteEEPROM(byte address, byte value)
{
    while(EECON1bits.WR) { }
    EECON1 = 0x00;
    EEADR = address;
    EEDATA = value;
    StartWrite();
}

void ReadVersion(void)
{
    dataPacket._byte[2] = MINOR_VERSION;
    dataPacket._byte[3] = MAJOR_VERSION;
}

void ReadProgMem(void)
{
    for(counter = 0; counter < dataPacket.len; counter++)
    {
        LoadTablePointer(counter);
        BootTableRead();
        dataPacket.data[counter] = TABLAT;
    }
    TBLPTRU = 0;
}

void WriteProgMem(void)
{
    /* Preserve the host's 16-byte write units and alignment. The device
     * has 32 holding registers; a commit is issued after each 16 bytes,
     * as in the C18 implementation. Do not increment TBLPTR before WR:
     * it must still point into the block containing the loaded latches.
     */
    dataPacket.ADR.low &= 0xF0;
    EECON1 = 0x80;        /* EEPGD=1, CFGS=0, FREE=0. */
    for(counter = 0; counter < dataPacket.len; counter++)
    {
        LoadTablePointer(counter);
        TABLAT = dataPacket.data[counter];
        BootTableWrite();
        if((counter & 0x0F) == 0x0F)
            StartWrite();
    }
    TBLPTRU = 0;
}

void EraseProgMem(void)
{
    /* len counts 64-byte erase blocks; hardware ignores address bits 5:0. */
    EECON1 = 0x90;        /* EEPGD=1, CFGS=0, FREE=1. */
    for(counter = 0; counter < dataPacket.len; counter++)
    {
        LoadTablePointer((word)((word)counter << 6));
        StartWrite();
    }
    TBLPTRU = 0;
}

void ReadEE(void)
{
    for(counter = 0; counter < dataPacket.len; counter++)
        dataPacket.data[counter] =
            BootReadEEPROM((byte)(dataPacket.ADR.low + counter));
}

void WriteEE(void)
{
    for(counter = 0; counter < dataPacket.len; counter++)
        BootWriteEEPROM((byte)(dataPacket.ADR.low + counter),
                        dataPacket.data[counter]);
}

void WriteConfig(void)
{
    EECON1 = 0xC0;        /* EEPGD=1, CFGS=1, FREE=0. */
    for(counter = 0; counter < dataPacket.len; counter++)
    {
        LoadTablePointer(counter);
        TABLAT = dataPacket.data[counter];
        BootTableWrite();
        StartWrite();
    }
    TBLPTRU = 0;
}

void BootService(void)
{
    BlinkUSBStatus();
    if((usb_device_state < CONFIGURED_STATE)||(UCONbits.SUSPND==1)) return;
    
    if(trf_state == SENDING_RESP)
    {
        if(!mBootTxIsBusy())
        {
            BOOT_BD_OUT.Cnt = sizeof(dataPacket);
            mUSBBufferReady(BOOT_BD_OUT);
            trf_state = WAIT_FOR_CMD;
        }//end if
        return;
    }//end if
    
    if(!mBootRxIsBusy())
    {
        counter = 0;
        switch(dataPacket.CMD)
        {
            case READ_VERSION:
                ReadVersion();
                counter=0x04;
                break;

            case READ_FLASH:
            case READ_CONFIG:
                ReadProgMem();
                counter+=0x05;
                break;

            case WRITE_FLASH:
                WriteProgMem();
                counter=0x01;
                break;

            case ERASE_FLASH:
                EraseProgMem();
                counter=0x01;
                break;

            case READ_EEDATA:
                ReadEE();
                counter+=0x05;
                break;

            case WRITE_EEDATA:
                WriteEE();
                counter=0x01;
                break;

            case WRITE_CONFIG:
                WriteConfig();
                counter=0x01;
                break;
            
            case RESET:
                //When resetting, make sure to drop the device off the bus
                //for a period of time. Helps when the device is suspended.
                UCONbits.USBEN = 0;
                big_counter = 0;
                while(--big_counter);
                
                BootReset();
                break;
            
            case UPDATE_LED:
                if(dataPacket.led_num == 3)
                {
                    mLED_3 = dataPacket.led_status;
                    counter = 0x01;
                }//end if
                if(dataPacket.led_num == 4)
                {
                    mLED_4 = dataPacket.led_status;
                    counter = 0x01;
                }//end if
                break;
                
            default:
                break;
        }//end switch()
        trf_state = SENDING_RESP;
        if(counter != 0)
        {
            BOOT_BD_IN.Cnt = counter;
            mUSBBufferReady(BOOT_BD_IN);
        }//end if
    }//end if
}//end BootService

/******************************************************************************
 * Function:        void BlinkUSBStatus(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        BlinkUSBStatus turns on and off LEDs corresponding to
 *                  the USB device state.
 *
 * Note:            mLED macros can be found in io_cfg.h
 *                  usb_device_state is declared in usbmmap.c and is modified
 *                  in usbdrv.c, usbctrltrf.c, and usb9.c
 *****************************************************************************/
void BlinkUSBStatus(void)
{
    static word led_count=0;
    
    if(led_count == 0)led_count = 20000U;
    led_count--;

    #define mLED_Both_Off()         {mLED_1_Off();mLED_2_Off();}
    #define mLED_Both_On()          {mLED_1_On();mLED_2_On();}
    #define mLED_Only_1_On()        {mLED_1_On();mLED_2_Off();}
    #define mLED_Only_2_On()        {mLED_1_Off();mLED_2_On();}

    if(UCONbits.SUSPND == 1)
    {
        if(led_count==0)
        {
            mLED_1_Toggle();
            mLED_2 = mLED_1;        // Both blink at the same time
        }//end if
    }
    else
    {
        if(usb_device_state == DETACHED_STATE)
        {
            mLED_Both_Off();
        }
        else if(usb_device_state == ATTACHED_STATE)
        {
            mLED_Both_On();
        }
        else if(usb_device_state == POWERED_STATE)
        {
            mLED_Only_1_On();
        }
        else if(usb_device_state == DEFAULT_STATE)
        {
            mLED_Only_2_On();
        }
        else if(usb_device_state == ADDRESS_STATE)
        {
            if(led_count == 0)
            {
                mLED_1_Toggle();
                mLED_2_Off();
            }//end if
        }
        else if(usb_device_state == CONFIGURED_STATE)
        {
            if(led_count==0)
            {
                mLED_1_Toggle();
                mLED_2 = !mLED_1;       // Alternate blink                
            }//end if
        }//end if(...)
    }//end if(UCONbits.SUSPND...)

}//end BlinkUSBStatus
/** EOF boot.c ***************************************************************/
