/* Author               Date        Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * Rafael Fernandez    10/03/07     Original.
 * Andres Aguirre      27/03/07
 * Alan Aguiar         31/07/13
 ********************************************************************/

/** I N C L U D E S **********************************************************/
#include <xc.h>
#include "pnp.h"
#include "system/typedefs.h"
#include "system/usb/usb.h"
#include "user/defines.h"
#include "user/loaderModule.h"
#include "io_cfg.h"             // I/O pin mapping
#include "user/handlerManager.h"
#include "usr_motors.h"

/** V A R I A B L E S ********************************************************/
unsigned char ram_max_ep_number;
epHandlerMapItem epHandlerMap[MAX_HANDLERS];
HM_DATA_PACKET_HEADER hmDataPacketHeader;
byte* HandlerReceiveBuffer[MAX_HANDLERS];
void (*handlerReceivedFuncion[MAX_HANDLERS]) (byte*, byte, byte); //arreglo de punteros a las funcioens received de los modulos
HANDLER_OPTYPE hn_opType;
/* Active modules receive/reply synchronously from USBGenRead2 in main.
 * Ordinary RAM is never shared with the USB DMA engine. These buffers must
 * not be used by ISR callbacks or retained for asynchronous responses. */
static byte receiveData[PACKET_DATA_SIZE];
static byte transmitData[PACKET_DATA_SIZE];

/** P R I V A T E  P R O T O T Y P E S ***************************************/

/** D E C L A R A T I O N S **************************************************/

void setHandlerReceiveBuffer(byte handler, byte *rb){
    HandlerReceiveBuffer[handler] = rb;
}

void unsetHandlerReceiveBuffer(byte handler){
    HandlerReceiveBuffer[handler] = 0;
}

void setHandlerReceiveFunction(byte handler, void (*pf) (byte*,byte,byte)){
    handlerReceivedFuncion[handler] = pf;
}

void unsetHandlerReceiveFunction(byte handler){
    handlerReceivedFuncion[handler] = 0;
}

void USBGenRead2(void){
    byte len;
    byte handler;
    byte i;
    byte ep = 1;
    volatile HM_DATA_PACKET_HEADER* dph;

    if((usb_device_state < CONFIGURED_STATE)||(UCONbits.SUSPND== (unsigned) 1)) return;

    if(!EPOUT_IS_BUSY(ep)){
        len = EPOUT_SIZE(ep);
        /* Require the complete transport header and at least a command byte.
         * Use the received USB length, as before; permit padded packets.
         * Reject invalid counts rather than clipping or underflowing them. */
        if (len > SIZE__HM_DATA_PACKET_HEADER && len <= PACKET_MTU) {
            dph = (volatile HM_DATA_PACKET_HEADER*)EPBUFFEROUT(ep);
            handler = dph->handlerNumber;
            if (handler < MAX_HANDLERS && !epHandlerMap[handler].ep.empty &&
                handlerReceivedFuncion[handler] != 0 &&
                epHandlerMap[handler].ep.EPNum == ep) {
                /* Keep OUT CPU-owned until the previous reply is consumed.
                 * Do not execute a command whose reply cannot yet be sent. */
                if (EPIN_IS_BUSY(ep)) return;
                len = (byte)(len - SIZE__HM_DATA_PACKET_HEADER);
                for (i = 0; i < len; ++i)
                    receiveData[i] = EPBUFFEROUT(ep)[i + SIZE__HM_DATA_PACKET_HEADER];
                handlerReceivedFuncion[handler](receiveData, len, handler);
            }
        }

        /* Re-arm OUT even after rejecting a packet, so reception can resume. */
        EPOUT_SIZE(ep) = getEPSizeOUT(ep);
        mUSBBufferReady2(EPOUT_BDT(ep));
    }
}

void USBGenWrite2(byte handler, byte len) {
    byte i;
    byte ep;
    volatile byte *buffer;

    if (len == 0u || handler >= MAX_HANDLERS) return;
    if (usb_device_state < CONFIGURED_STATE || UCONbits.SUSPND) return;
    if (epHandlerMap[handler].ep.empty) return;
    ep = epHandlerMap[handler].ep.EPNum;
    /* USBInitEPs currently configures only endpoint 1. */
    if (ep != 1u || EPIN_IS_BUSY(ep)) return;
    if (len > PACKET_DATA_SIZE) len = PACKET_DATA_SIZE;

    buffer = EPBUFFERIN(ep);
    hn_opType.handlerNumber = handler;
    hn_opType.operationType = SEND;
    buffer[0] = hn_opType.hn_op;
    buffer[1] = (byte)(len + SIZE__HM_DATA_PACKET_HEADER);
    buffer[2] = 0;
    for (i = 0; i < len; ++i)
        buffer[i + SIZE__HM_DATA_PACKET_HEADER] = transmitData[i];
    EPIN_SIZE(ep) = (byte)(len + SIZE__HM_DATA_PACKET_HEADER);
    /* Publish ownership only after all volatile buffer writes are complete. */
    mUSBBufferReady2(EPIN_BDT(ep));
}

byte newHandlerTableEntry(byte endPIn, const uTab *uTableDirection){
    byte i = 0;
    while (i < MAX_HANDLERS){
        if (epHandlerMap[i].ep.empty == (unsigned) 1) {
            epHandlerMap[i].ep.endPoint = endPIn;
            epHandlerMap[i].ep.empty = 0;
            epHandlerMap[i].uTableDirection = uTableDirection;
            return i;
        }
        i++;
    }
    return ERROR;
}

byte newHandlerTableEntryForcingHandler(byte endPIn, const uTab *uTableDirection, byte handler){
    if (epHandlerMap[handler].ep.empty == (unsigned) 1) {
        epHandlerMap[handler].ep.endPoint = endPIn;
        epHandlerMap[handler].ep.empty = 0;
        epHandlerMap[handler].uTableDirection = uTableDirection;
        return handler;
    } else {
        return ERROR;
    }
}

BOOL existsTableEntry(const uTab *uTableDirection){
    byte i=0;
    while (i<MAX_HANDLERS){
        if (uTableDirection != 0 && !epHandlerMap[i].ep.empty &&
            epHandlerMap[i].uTableDirection == uTableDirection) {
            return TRUE;
        }
        i++;
    }
    return FALSE;
}

byte handlerFromTableEntry(const uTab *uTableDirection){
    byte i = 0;
    while (i < MAX_HANDLERS){
        if (uTableDirection != 0 && !epHandlerMap[i].ep.empty &&
            epHandlerMap[i].uTableDirection == uTableDirection) {
            return i;
        }
        i++;
    }
    return ERROR;
}

void initHandlerTable() {
    byte i;
    for(i=0;i<MAX_HANDLERS;i++){
        epHandlerMap[i].ep.empty = 1;
        epHandlerMap[i].uTableDirection = 0;
    }
    //cargo el ROM_MAX_EP_NUMBER en ram
    ram_max_ep_number = ROM_MAX_EP_NUMBER;
}

void initHandlerManager(void){
    byte modulename[8];
    initHandlerTable();      //Initialize table index(handler)=>endpoint

    /* Staticaly Initialized modules */

    /* Admin module; Handler=0 */
    modulename[0]='a'; modulename[1]='d'; modulename[2]='m'; modulename[3]='i';
    modulename[4]='n'; modulename[5]=0  ; modulename[6]=0  ; modulename[7]=0  ;
    epHandlerMap[0].ep = getAdminEndpoint(); // Admin endpoint
    epHandlerMap[0].uTableDirection = getUserTableDirection(modulename); // ModuleType=0;
    adminModuleInit(0);

    /* PNP module ; Handler=7 */
    modulename[0]='p'; modulename[1]='n'; modulename[2]='p'; modulename[3]=0 ;
    modulename[4]=0; modulename[5]=0  ; modulename[6]=0  ; modulename[7]=0  ;
    epHandlerMap[MAX_PORTS + 1].ep = getPnPEndpoint();
    epHandlerMap[MAX_PORTS + 1].uTableDirection = getUserTableDirection(modulename);
    autoDetectWheels();
    PNPInit(MAX_PORTS + 1);
}

respType removeHandlerTableEntry(byte handler){
    pUserFunc releaseFunction;
    if (handler < MAX_HANDLERS && epHandlerMap[handler].ep.empty == (unsigned) 0){
        epHandlerMap[handler].ep.empty = 1;
        releaseFunction = getModuleReleaseDirection(epHandlerMap[handler].uTableDirection);
        releaseFunction(handler);
        epHandlerMap[handler].uTableDirection = 0;
        return ACK;
    }
    else{
        return NACK;
    }
}

byte removeAllOpenModules(void){
    byte handler;
    for(handler = 0; handler < MAX_HANDLERS; handler++){
        removeHandlerTableEntry(handler + 1);
    }
    return ACK;
}

byte* getSharedBuffer(byte handler){
    /* All active modules share the foreground reply scratch buffer.
     * This is safe even before USBInitEPs initializes the DMA descriptors. */
    if (handler >= MAX_HANDLERS) return 0;
    return transmitData;
}

byte getEPSizeOUT(byte ep){
    return USBGEN_EP_SIZE;
}


byte getEPSizeIN(byte ep){
    return USBGEN_EP_SIZE;
}

byte getMaxHandler(void){
    byte handler, max_handler = 0;
    for(handler = 0; handler < MAX_HANDLERS; handler++){
        if (epHandlerMap[handler].ep.empty == (unsigned) 0){
            max_handler = handler;
        }
    }
    return max_handler;
}

/** EOF handlerManager.c ***************************************************************/

