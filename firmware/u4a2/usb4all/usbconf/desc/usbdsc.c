/** I N C L U D E S *************************************************/
#include "system/typedefs.h"
#include "system/usb/usb.h"
/** C O N S T A N T S ************************************************/

//	 Device Descriptor
const USB_DEV_DSC device_dsc=
{
    sizeof(USB_DEV_DSC),    // Size of this descriptor in bytes
    DSC_DEV,                // DEVICE descriptor type
    0x0200,				// USB Spec Release Number in BCD format
    0x0,				// Class Code
    0x0,				// Subclass code
    0x0,				// Protocol code
    EP0_BUFF_SIZE,				// Max packet size for EP0, see usbcfg.h
    0x4d8,				// Vendor ID
    0xc,				// Product ID: PICDEM FS USB (DEMO Mode)
    0x0,				// Device release number in BCD format
    0x1,				// Manufacturer string index
    0x2,				// Product string index
    0x3,				// Device serial number string index
    0x1,				// Number of possible configurations
};
//		 Configuration 1 Descriptor
const CFG01 cfg01=
{
    // Configuration Descriptor
    sizeof(USB_CFG_DSC),    // Size of this descriptor in bytes
    DSC_CFG,                // CONFIGURATION descriptor type
    sizeof(cfg01),          // Total length of data for this cfg
1,                      // Number of interfaces in this cfg
1,                      // Index value of this configuration
0,                      // Configuration string index
_DEFAULT,               // Attributes, see usbdefs_std_dsc.h
250,                    // Max power consumption (2X mA)
// Interface Descriptor
sizeof(USB_INTF_DSC),   // Size of this descriptor in bytes
DSC_INTF,               // INTERFACE descriptor type
0,                      // Interface Number
0,                      // Alternate Setting Number
2,                      // Number of endpoints in this intf
0x0,                   // Class code
0x0,                   // Subclass code
0x0,                   // Protocol code
0,                      // Interface string index

//				 Endpoint Descriptors
sizeof(USB_EP_DSC),DSC_EP,_EP01_OUT,_BULK,USBGEN_EP_SIZE,255,
sizeof(USB_EP_DSC),DSC_EP,_EP01_IN,_BULK,USBGEN_EP_SIZE,255};
//sizeof(USB_EP_DSC),DSC_EP,_EP02_OUT,_BULK,USBGEN_EP_SIZE,255,
//sizeof(USB_EP_DSC),DSC_EP,_EP02_IN,_BULK,USBGEN_EP_SIZE,255,
//sizeof(USB_EP_DSC),DSC_EP,_EP03_OUT,_ISO,USBGEN_EP_SIZE,6,
//sizeof(USB_EP_DSC),DSC_EP,_EP03_IN,_ISO,USBGEN_EP_SIZE,6};
const struct{byte bLength;byte bDscType;word string[1];}sd000={
sizeof(sd000),DSC_STR,0x409};
const struct{byte bLength;byte bDscType;word string[36];}sd001={
sizeof(sd001),DSC_STR,'w','w','w','.','f','i','n','g','.','e','d','u','.','u','y','/','i','n','c','o','/','p','r','o','y','e','c','t','o','s','/','b','u','t','i','a'};
const struct{byte bLength;byte bDscType;word string[26];}sd002={
sizeof(sd002),DSC_STR,'U','S','B','4','B','u','t','i','a',' ','2','0','1','1',' ','F','I','N','G',' ','U','d','e','l','a','R'};
const struct{byte bLength;byte bDscType;word string[8];}sd003={
sizeof(sd003),DSC_STR,'0','0','0','0','0','0','0','2'};
//dejo estos punteros en una posicion fija de memoria
const unsigned char *const USB_CD_Ptr[]={(const unsigned char*)&cfg01,(const unsigned char*)&cfg01};
const unsigned char *const USB_SD_Ptr[]={(const unsigned char*)&sd000,(const unsigned char*)&sd001,(const unsigned char*)&sd002,(const unsigned char*)&sd003};
