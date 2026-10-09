/*********************************************************************
 *
 *                Microchip USB C18 Firmware Version 1.0
 *
 *********************************************************************
 * FileName:        usbmmap.c
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
 * Rawin Rojvanit       11/19/04    Original.
 ********************************************************************/

/******************************************************************************
 * -usbmmap.c-
 * USB Memory Map
 * This file is the USB memory manager; it serves as a compile-time memory
 * allocator for the USB endpoints. It uses the compile time options passed
 * from usbcfg.h to instantiate endpoints and endpoint buffer.
 *
 * Each endpoint requires to have a set of Buffer Descriptor registers(BDT).
 * A BDT is 4-byte long and has a specific RAM location for each endpoint.
 * The BDT for endpoint 0 out is located at address 0x400 to 0x403.
 * The BDT for endpoint 0 in is located at address 0x404 to 0x407.
 * The BDT for endpoint 1 out is located at address 0x408 to 0x40B.
 * and so on... The above allocation assumes the Ping-Pong Buffer Mode 0 is
 * used. These locations are already hard-wired in the silicon. The point
 * of doing instantiation, i.e. volatile BOOT_FAR BDT ep0Bo BOOT_USB_AT(0x400);, is to provide the
 * C compiler a way to address each variable directly. This is very important
 * because when a register can be accessed directly, it saves execution time
 * and reduces program size.
 * 
 * Endpoints are defined using the endpoint number and the direction
 * of transfer. For simplicity, usbmmap.c only uses the endpoint number
 * in the BDT register allocation scheme. This means if the usbcfg.h states
 * that the MAX_EP_NUMBER is number 1, then four BDTs will be
 * instantiated: one each for endpoint0 in and endpoint0 out, which must
 * always be instantiated for control transfer by default, and one each sets
 * for endpoint1 in and endpoint1 out. The naming convention for instantiating
 * BDT is
 * 
 * ep<#>B<d>
 *
 * where # is the endpoint number, and d is the direction of
 * transfer, which could be either <i> or <o>.
 *
 * The USB memory manager uses MAX_EP_NUMBER, as defined in usbcfg.h, to define
 * the endpoints to be instantiated. This represents the highest endpoint
 * number to be allocated, not how many endpoints are used. Since the BDTs for
 * endpoints have hardware-assigned addresses in Bank 4, setting this value too
 * high may lead to inefficient use of data RAM. For example, if an application
 * uses only endpoints EP0 and EP4, then the MAX_EP_NUMBER is 4, and not 2.
 * The in-between endpoint BDTs in this example (EP1, EP2, and EP3) go unused,
 * and the 24 bytes of memory associated with them are wasted. It does not make
 * much sense to skip endpoints, but the final decision lies with the user.
 *
 * The next step is to assign the instantiated BDTs to different
 * USB functions. The firmware framework fundamentally assumes that every USB
 * function should know which endpoint it is using, i.e., the default control
 * transfer should know that it is using endpoint 0 in and endpoint 0 out.
 * A HID class can choose which endpoint it wants to use, but once chosen, it
 * should always know the number of the endpoint.
 *
 * The assignment of endpoints to USB functions is managed centrally
 * in usbcfg.h. This helps prevent the mistake of having more
 * than one USB function using the same endpoint. The "Endpoint Allocation"
 * section in usbcfg.h provides examples for how to map USB endpoints to USB
 * functions.
 * Quite a few things can be mapped in that section. There is no
 * one correct way to do the mapping and the user has the choice to
 * choose a method that is most suitable to the application.
 *
 * Typically, however, a user will want to map the following for a given
 * USB interface function:
 * 1. The USB interface ID
 * 2. The endpoint control registers (UEPn)
 * 3. The BDT registers (ep<#>B<d>)
 * 4. The endpoint size
 *
 * Example: Assume a USB device class "foo", which uses one out endpoint
 *          of size 64-byte and one in endpoint of size 64-byte, then:
 *
 * #define FOO_INTF_ID          0x00
 * #define FOO_UEP              UEP1
 * #define FOO_BD_OUT           ep1Bo
 * #define FOO_BD_IN            ep1Bi
 * #define FOO_EP_SIZE          64
 *
 * The mapping above has chosen class "foo" to use endpoint 1.
 * The names are arbitrary and can be anything other than FOO_??????.
 * For abstraction, the code for class "foo" should use the abstract
 * definitions of FOO_BD_OUT,FOO_BD_IN, and not ep1Bo or ep1Bi.
 *
 * Note that the endpoint size defined in the usbcfg.h file is again
 * used in the usbmmap.c file. This shows that the relationship between
 * the two files are tightly related.
 * 
 * The endpoint buffer for each USB function must be located in the
 * dual-port RAM area and has to come after all the BDTs have been
 * instantiated. An example declaration is:
 * volatile BOOT_FAR unsigned char[FOO_EP_SIZE] data;
 *
 * The 'volatile' keyword tells the compiler not to perform any code
 * optimization on this variable because its content could be modified
 * by the hardware. The 'far' keyword tells the compiler that this variable
 * is not located in the Access RAM area (0x000 - 0x05F).
 *
 * For the variable to be globally accessible by other files, it should be
 * declared in the header file usbmmap.h as an extern definition, such as
 * extern volatile BOOT_FAR unsigned char[FOO_EP_SIZE] data;
 *
 * Conclusion:
 * In a short summary, the dependencies between usbcfg and usbmmap can
 * be shown as:
 *
 * usbcfg[MAX_EP_NUMBER] -> usbmmap
 * usbmmap[ep<#>B<d>] -> usbcfg
 * usbcfg[EP size] -> usbmmap
 * usbcfg[abstract ep definitions] -> usb9/hid/cdc/etc class code
 * usbmmap[endpoint buffer variable] -> usb9/hid/cdc/etc class code
 *
 * Data mapping provides a means for direct addressing of BDT and endpoint
 * buffer. This means less usage of pointers, which equates to a faster and
 * smaller program code.
 *
 *****************************************************************************/

/** I N C L U D E S **********************************************************/
#include "typedefs.h"
#include "usb.h"

/** U S B  G L O B A L  V A R I A B L E S ************************************/
#if !defined(__XC8)
#pragma udata
#endif
byte usb_device_state;          // Device States: DETACHED, ATTACHED, ...
USB_DEVICE_STATUS usb_stat;     // Global USB flags
byte usb_active_cfg;            // Value of current configuration
byte usb_alt_intf[MAX_NUM_INT]; // Array to keep track of the current alternate
                                // setting for each interface ID

/** U S B  F I X E D  L O C A T I O N  V A R I A B L E S *********************/
#if !defined(__XC8)
#pragma udata usbram4=0x400     //See Linker Script,usb4:0x400-0x4FF(256-byte)
#endif

/******************************************************************************
 * Section A: Buffer Descriptor Table
 * - 0x400 - 0x4FF(max)
 * - MAX_EP_NUMBER is defined in autofiles\usbcfg.h
 * - BDT data type is defined in system\usb\usbmmap.h
 *****************************************************************************/

#if(0 <= MAX_EP_NUMBER)
volatile BOOT_FAR BDT ep0Bo BOOT_USB_AT(0x400);         //Endpoint #0 BD Out
volatile BOOT_FAR BDT ep0Bi BOOT_USB_AT(0x404);         //Endpoint #0 BD In
#endif

#if(1 <= MAX_EP_NUMBER)
volatile BOOT_FAR BDT ep1Bo BOOT_USB_AT(0x408);         //Endpoint #1 BD Out
volatile BOOT_FAR BDT ep1Bi BOOT_USB_AT(0x40C);         //Endpoint #1 BD In
#endif

#if(2 <= MAX_EP_NUMBER)
volatile BOOT_FAR BDT ep2Bo BOOT_USB_AT(0x410);         //Endpoint #2 BD Out
volatile BOOT_FAR BDT ep2Bi BOOT_USB_AT(0x414);         //Endpoint #2 BD In
#endif

#if(3 <= MAX_EP_NUMBER)
volatile BOOT_FAR BDT ep3Bo BOOT_USB_AT(0x418);         //Endpoint #3 BD Out
volatile BOOT_FAR BDT ep3Bi BOOT_USB_AT(0x41C);         //Endpoint #3 BD In
#endif

#if(4 <= MAX_EP_NUMBER)
volatile BOOT_FAR BDT ep4Bo BOOT_USB_AT(0x420);         //Endpoint #4 BD Out
volatile BOOT_FAR BDT ep4Bi BOOT_USB_AT(0x424);         //Endpoint #4 BD In
#endif

#if(5 <= MAX_EP_NUMBER)
volatile BOOT_FAR BDT ep5Bo BOOT_USB_AT(0x428);         //Endpoint #5 BD Out
volatile BOOT_FAR BDT ep5Bi BOOT_USB_AT(0x42C);         //Endpoint #5 BD In
#endif

#if(6 <= MAX_EP_NUMBER)
volatile BOOT_FAR BDT ep6Bo BOOT_USB_AT(0x430);         //Endpoint #6 BD Out
volatile BOOT_FAR BDT ep6Bi BOOT_USB_AT(0x434);         //Endpoint #6 BD In
#endif

#if(7 <= MAX_EP_NUMBER)
volatile BOOT_FAR BDT ep7Bo BOOT_USB_AT(0x438);         //Endpoint #7 BD Out
volatile BOOT_FAR BDT ep7Bi BOOT_USB_AT(0x43C);         //Endpoint #7 BD In
#endif

#if(8 <= MAX_EP_NUMBER)
volatile BOOT_FAR BDT ep8Bo BOOT_USB_AT(0x440);         //Endpoint #8 BD Out
volatile BOOT_FAR BDT ep8Bi BOOT_USB_AT(0x444);         //Endpoint #8 BD In
#endif

#if(9 <= MAX_EP_NUMBER)
volatile BOOT_FAR BDT ep9Bo BOOT_USB_AT(0x448);         //Endpoint #9 BD Out
volatile BOOT_FAR BDT ep9Bi BOOT_USB_AT(0x44C);         //Endpoint #9 BD In
#endif

#if(10 <= MAX_EP_NUMBER)
volatile BOOT_FAR BDT ep10Bo BOOT_USB_AT(0x450);        //Endpoint #10 BD Out
volatile BOOT_FAR BDT ep10Bi BOOT_USB_AT(0x454);        //Endpoint #10 BD In
#endif

#if(11 <= MAX_EP_NUMBER)
volatile BOOT_FAR BDT ep11Bo BOOT_USB_AT(0x458);        //Endpoint #11 BD Out
volatile BOOT_FAR BDT ep11Bi BOOT_USB_AT(0x45C);        //Endpoint #11 BD In
#endif

#if(12 <= MAX_EP_NUMBER)
volatile BOOT_FAR BDT ep12Bo BOOT_USB_AT(0x460);        //Endpoint #12 BD Out
volatile BOOT_FAR BDT ep12Bi BOOT_USB_AT(0x464);        //Endpoint #12 BD In
#endif

#if(13 <= MAX_EP_NUMBER)
volatile BOOT_FAR BDT ep13Bo BOOT_USB_AT(0x468);        //Endpoint #13 BD Out
volatile BOOT_FAR BDT ep13Bi BOOT_USB_AT(0x46C);        //Endpoint #13 BD In
#endif

#if(14 <= MAX_EP_NUMBER)
volatile BOOT_FAR BDT ep14Bo BOOT_USB_AT(0x470);        //Endpoint #14 BD Out
volatile BOOT_FAR BDT ep14Bi BOOT_USB_AT(0x474);        //Endpoint #14 BD In
#endif

#if(15 <= MAX_EP_NUMBER)
volatile BOOT_FAR BDT ep15Bo BOOT_USB_AT(0x478);        //Endpoint #15 BD Out
volatile BOOT_FAR BDT ep15Bi BOOT_USB_AT(0x47C);        //Endpoint #15 BD In
#endif

/******************************************************************************
 * Section B: EP0 Buffer Space
 ******************************************************************************
 * - Two buffer areas are defined:
 *
 *   A. CTRL_TRF_SETUP
 *      - Size = EP0_BUFF_SIZE as defined in autofiles\usbcfg.h
 *      - Detailed data structure allows direct adddressing of bits and bytes.
 *
 *   B. CTRL_TRF_DATA
 *      - Size = EP0_BUFF_SIZE as defined in autofiles\usbcfg.h
 *      - Data structure allows direct adddressing of the first 8 bytes.
 *
 * - Both data types are defined in system\usb\usbdefs\usbdefs_ep0_buff.h
 *****************************************************************************/
volatile BOOT_FAR CTRL_TRF_SETUP SetupPkt BOOT_USB_AT(0x410);
volatile BOOT_FAR CTRL_TRF_DATA CtrlTrfData BOOT_USB_AT(0x418);

/******************************************************************************
 * Section C: Buffer
 ******************************************************************************
 *
 *****************************************************************************/
volatile BOOT_FAR BOOT_DATA_PACKET dataPacket BOOT_USB_AT(0x420);

#if !defined(__XC8)
#pragma udata
#endif

/** EOF usbmmap.c ************************************************************/

#if defined(__XC8)
#if MAX_EP_NUMBER != 1 || EP0_BUFF_SIZE != 8 || MODE_PP != _PPBM0
#error "Update the fixed USB layout before changing endpoints or ping-pong mode."
#endif
/* Compiled by the target compiler, not inferred from host type sizes. */
#define USB_CHECK(name, expression) typedef char name[(expression) ? 1 : -1]
USB_CHECK(usb_byte_size, sizeof(byte) == 1);
USB_CHECK(usb_word_size, sizeof(word) == 2);
USB_CHECK(usb_dword_size, sizeof(dword) == 4);
USB_CHECK(usb_bd_stat_size, sizeof(BD_STAT) == 1);
USB_CHECK(usb_bd_size, sizeof(BDT) == 4);
USB_CHECK(usb_bd_address_offset, offsetof(BDT, ADR) == 2);
USB_CHECK(usb_bd_address_size, sizeof(((BDT *)0)->ADR) == 2);
USB_CHECK(usb_setup_size, sizeof(CTRL_TRF_SETUP) == 8);
USB_CHECK(usb_control_size, sizeof(CTRL_TRF_DATA) == 8);
USB_CHECK(usb_device_descriptor_size, sizeof(USB_DEV_DSC) == 18);
USB_CHECK(usb_config_descriptor_size, sizeof(USB_CFG_DSC) == 9);
USB_CHECK(usb_interface_descriptor_size, sizeof(USB_INTF_DSC) == 9);
USB_CHECK(usb_endpoint_descriptor_size, sizeof(USB_EP_DSC) == 7);
USB_CHECK(usb_configuration_size, sizeof(BOOT_CFG_DSC) == 32);
USB_CHECK(usb_language_descriptor_size, sizeof(BOOT_LANG_DSC) == 4);
USB_CHECK(usb_application_vector, RM_RESET_VECTOR == 0x08C0);
USB_CHECK(usb_high_vector, RM_HIGH_INTERRUPT_VECTOR == 0x08C8);
USB_CHECK(usb_low_vector, RM_LOW_INTERRUPT_VECTOR == 0x08D8);
#undef USB_CHECK
#endif
