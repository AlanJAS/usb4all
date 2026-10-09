/* Author             									  Date        Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * andrew
 *****************************************************************************/

/** I N C L U D E S **********************************************************/
#include <xc.h>
#include "system/typedefs.h"
#include "user/usr_butia.h"
#include "user/handlerManager.h"
#include "usr_motors.h"

/** V A R I A B L E S ********************************************************/
byte* sendBufferusrButia; // buffer to send data

/** P R I V A T E  P R O T O T Y P E S ***************************************/
void UserButiaInit(byte handler);
void UserButiaReceived(byte*, byte, byte);
void UserButiaRelease(byte handler);

// Table used by te framework to get a fixed reference point to the user module functions defined by the framework
/** USER MODULE REFERENCE*****************************************************/
const uTab UserButiaModuleTable = {&UserButiaInit,&UserButiaRelease,"butia"};

/** D E C L A R A T I O N S **************************************************/

/******************************************************************************
 * Function:        UserButiaInit(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        This function is initialices the resources that the user module needs to work, it is called by the framework
 *					when the module is opened
 *
 * Note:            None
 *****************************************************************************/

void UserButiaInit(byte handler){
    // add my receive function to the handler module, to be called automatically when the pc sends data to the user module
    setHandlerReceiveFunction(handler,&UserButiaReceived);
    // initialize the send buffer, used to send data to the PC
    sendBufferusrButia = getSharedBuffer(handler);
}//end UserButiaInit


/******************************************************************************
 * Function:        UserButiaRelease(byte i)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        This function release all the resources that the user module used, it is called by the framework
 *					when the module is close
 *
 * Note:            None
 *****************************************************************************/

void UserButiaRelease(byte handler){
    unsetHandlerReceiveBuffer(handler);
    unsetHandlerReceiveFunction(handler);
}

/******************************************************************************
 * Function:        UserButiaReceived(byte* recBuffPtr, byte len)
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

void UserButiaReceived(byte* recBuffPtr, byte len, byte handler){
    byte UserButiaCounter = 0;
    int data_received = 3;

    if (len == 0u) return;

    switch(((BUTIA_DATA_PACKET*)recBuffPtr)->CMD)
    {
        case U4A_USR_BUTIA_READ_VERSION_BUTIA:
            ((BUTIA_DATA_PACKET*)sendBufferusrButia)->_byte[0] = ((BUTIA_DATA_PACKET*)recBuffPtr)->_byte[0];
            ((BUTIA_DATA_PACKET*)sendBufferusrButia)->_byte[1] = BUTIA_VERSION;
            UserButiaCounter=0x02;
            break;
        case U4A_USR_BUTIA_GET_VOLT:
            ((BUTIA_DATA_PACKET*)sendBufferusrButia)->_byte[0] = ((BUTIA_DATA_PACKET*)recBuffPtr)->_byte[0];
            getVoltage(&data_received);
            ((BUTIA_DATA_PACKET*)sendBufferusrButia)->_byte[1] = (byte) (data_received % 256);
            UserButiaCounter=0x02;
            break;
        case U4A_USR_BUTIA_RESET:
            Reset();
            break;

        default:
            break;
    }/*end switch(s)*/

    USBGenWrite2(handler, UserButiaCounter);

}/*end UserButiaReceived*/

/** EOF usr_butia.c ***************************************************************/
