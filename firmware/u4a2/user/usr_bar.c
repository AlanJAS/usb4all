/* Author                                           Date        Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * Enrique Madruga                                 31/10/12    Original Halloween.
 * Andrés Aguirre
 *****************************************************************************/

/** I N C L U D E S **********************************************************/
#include <p18cxxx.h>
#include <usart.h>
#include "system/typedefs.h"
#include "system/usb/usb.h"
#include "user/usr_bar.h"
#include "io_cfg.h"              /* I/O pin mapping*/
#include "user/handlerManager.h"
#include "dynamicPolling.h"
#include "user/usb4butia.h"     /**/

#define TRUE 1
#define FALSE 0

#define WAIT_RISING_EDGE_STATE  1
#define WAIT_FALLING_EDGE_STATE 2
#define COUNTING_STATE          3
#define END_COUNTING_STATE      4

#define MAX_DWORD 4294967295
#define MS 636
#define TIME_UNIT  65500
//#define TIME_UNIT  1

/** V A R I A B L E S ********************************************************/
#pragma udata

byte* sendBufferUsrBar; /* buffer to send data*/
byte state;
byte start;
byte bar_handler;
WORD bar_value;
DWORD tics;
byte termine;

/** P R I V A T E  P R O T O T Y P E S ***************************************/
void UserBarProcessIO(void);
void UserBarInit(byte i);
void UserBarReceived(byte*, byte, byte);
void UserBarRelease(byte i);

/* Table used by te framework to get a fixed reference point to the user module functions defined by the framework */
/** USER MODULE REFERENCE ****************************************************/
#pragma romdata user
const uTab userBarModuleTable = {&UserBarInit, &UserBarRelease, "bar"};
#pragma code

/** D E C L A R A T I O N S **************************************************/
#pragma code module

/******************************************************************************
 * Function:        UserGreyInit(void)
 *
 * PreCondition:    None
 *
 * Input:           module handler identifier
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        This function initialices the resources that the user
 *                  module needs to work, it is called by the framework when
 *                  the module is opened.
 *
 * Note:            None
 *****************************************************************************/
void UserBarInit(byte usrBarHandler) {
    /* add my receive function to the handler module, to be called automatically
     * when the pc sends data to the user module */
    setHandlerReceiveFunction(usrBarHandler, &UserBarReceived);
    /* initialize the send buffer, used to send data to the PC */
    sendBufferUsrBar = getSharedBuffer(usrBarHandler);
    /* get port where sensor/actuator is connected and set to IN/OUT mode*/
    getPortDescriptor(usrBarHandler)->change_port_direction(IN);
    start = FALSE;
    state = END_COUNTING_STATE;
    bar_handler = usrBarHandler;
    tics._dword=0;
    termine = TRUE;
}/*end UserBarInit*/


void tics_rtn(void){
    if (state==COUNTING_STATE){
        if (tics._dword==MAX_DWORD){
            tics._dword=0;
            registerT0eventInEvent(TIME_UNIT, &tics_rtn);
        }
        else{
            tics._dword++;
        }
    }
}

/******************************************************************************
 * Function:        UserGreyProcessIO(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        This function is registered in the dinamic polling, who
 *                  calls it periodically to process the IO interaction in the
 *                  PIC, it also can comunicate things to the pc by the USB.
 *
 * Note:            None
 *****************************************************************************/
void UserBarProcessIO(void) {
    if ((usb_device_state < CONFIGURED_STATE) || (UCONbits.SUSPND == (unsigned) 1)) return;    
    while(!termine){
        switch (state){
            case WAIT_RISING_EDGE_STATE:
                if(getPortDescriptor(bar_handler)->get_data_analog()._word > 30000){
                    state=WAIT_FALLING_EDGE_STATE;
                }
                break;
            case WAIT_FALLING_EDGE_STATE:
                if(getPortDescriptor(bar_handler)->get_data_analog()._word < 30000){
                    state=COUNTING_STATE;
                    tics._dword = 0;
                    registerT0event(TIME_UNIT, &tics_rtn);
                }
                break;
            case COUNTING_STATE:
                if(getPortDescriptor(bar_handler)->get_data_analog()._word > 30000){
                    state=END_COUNTING_STATE;
                }
                break;
            case END_COUNTING_STATE:
                if(start == TRUE){
                    state=WAIT_RISING_EDGE_STATE;
                    start=FALSE;
                    removePoolingFunction(&UserBarProcessIO);
                    termine=TRUE;
                    break;
                }
                break;
            default:
                break;
        }
    }
}/*end UserBarProcessIO*/

/******************************************************************************
 * Function:        UserBarRelease(byte i)
 *
 * PreCondition:    None
 *
 * Input:           module handler identifier
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        This function release all the resources that the user
 *                  module used, it is called by the framework
 *                  when the module is close.
 *
 * Note:            None
 *****************************************************************************/
void UserBarRelease(byte i) {
    unsetHandlerReceiveBuffer(i);
    unsetHandlerReceiveFunction(i);
}/*end UserGreyRelease*/

/******************************************************************************
 * Function:        UserGreyReceived(byte* recBuffPtr, byte len)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        This function manages the comunication with the pc
 *
 * Note:            None
 *****************************************************************************/
void UserBarReceived(byte* recBuffPtr, byte len, byte handler) {
    byte j;
    WORD data;
    byte userBarCounter = 0;
    switch (((BAR_DATA_PACKET*) recBuffPtr)->CMD) {
        case READ_VERSION:
            ((BAR_DATA_PACKET*) sendBufferUsrBar)->_byte[0] = ((BAR_DATA_PACKET*) recBuffPtr)->_byte[0];
            ((BAR_DATA_PACKET*) sendBufferUsrBar)->_byte[1] = BAR_MINOR_VERSION;
            ((BAR_DATA_PACKET*) sendBufferUsrBar)->_byte[2] = BAR_MAJOR_VERSION;
            userBarCounter = 0x03;
            break;

        case GET_VALUE:
            ((BAR_DATA_PACKET*) sendBufferUsrBar)->_byte[0] = ((BAR_DATA_PACKET*) recBuffPtr)->_byte[0];
            data = getPortDescriptor(handler)->get_data_analog();
            ((BAR_DATA_PACKET*) sendBufferUsrBar)->_byte[1] = LSB(data);
            ((BAR_DATA_PACKET*) sendBufferUsrBar)->_byte[2] = MSB(data);
            userBarCounter = 0x03;
            break;

        case START:
            start=TRUE;
            ((BAR_DATA_PACKET*) sendBufferUsrBar)->_byte[0] = ((BAR_DATA_PACKET*) recBuffPtr)->_byte[0];
            userBarCounter = 0x01;
            termine = FALSE;
            addPollingFunction(&UserBarProcessIO);
            break;

        case IS_COUNTING:
            if (state==COUNTING_STATE){
                ((BAR_DATA_PACKET*) sendBufferUsrBar)->_byte[1] = 0x01;
            }else{
                ((BAR_DATA_PACKET*) sendBufferUsrBar)->_byte[1] = 0x00;
            }
            userBarCounter = 0x02;
            break;

        case IS_DONE:
            if (state==END_COUNTING_STATE){
                ((BAR_DATA_PACKET*) sendBufferUsrBar)->_byte[1] = 0x01;
            }else{
                ((BAR_DATA_PACKET*) sendBufferUsrBar)->_byte[1] = 0x00;
            }
            userBarCounter = 0x02;
            break;

        case GET_TIME:
            ((BAR_DATA_PACKET*) sendBufferUsrBar)->_byte[0] = ((BAR_DATA_PACKET*) recBuffPtr)->_byte[0];
            ((BAR_DATA_PACKET*) sendBufferUsrBar)->_byte[1] = LOWER_LSB(tics);
            ((BAR_DATA_PACKET*) sendBufferUsrBar)->_byte[2] = LOWER_MSB(tics);
            ((BAR_DATA_PACKET*) sendBufferUsrBar)->_byte[3] = UPPER_LSB(tics);
            ((BAR_DATA_PACKET*) sendBufferUsrBar)->_byte[4] = UPPER_MSB(tics);
            userBarCounter = 0x05;
            break;
            
        case RESET:
            Reset();
            break;

        default:
            break;
    }/*end switch(s)*/

    if (userBarCounter != (byte) 0) {
        j = 255;
        while (mUSBGenTxIsBusy() && j-- > (byte) 0); /* pruebo un maximo de 255 veces */
            if (!mUSBGenTxIsBusy())
                USBGenWrite2(handler, userBarCounter);
    }/*end if*/
}/*end UserBarReceived*/

/** EOF usr_bar.c ***************************************************************/
