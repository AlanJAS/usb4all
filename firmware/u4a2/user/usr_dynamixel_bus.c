/* Author                                                   Date        Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *****************************************************************************/
 
/** I N C L U D E S **********************************************************/
#include <p18cxxx.h>
#include <usart.h>
#include <delays.h>
#include "system/typedefs.h"
#include "system/usb/usb.h"
#include "user/usr_ax12.h"
#include "io_cfg.h"              // I/O pin mapping
#include "user/handlerManager.h"
#include "dynamicPolling.h"   
#include "usb4all/proxys/T0Service.h"

  
/** V A R I A B L E S ********************************************************/
#pragma udata 

byte  usrDynamixelBusHandler;     // Handler number asigned to the module
byte* sendBufferUsrDynamixelBus; // buffer to send data

/** P R I V A T E  P R O T O T Y P E S ***************************************/
void UserDynamixelBusProcessIO(void);
void UserDynamixelBusInit(byte i);
void UserDynamixelBusReceived(byte*, byte);
void UserDynamixelBusRelease(byte i);
void UserDynamixelBusConfigure(void);

// Table used by te framework to get a fixed reference point to the user module functions defined by the framework 
/** USER MODULE REFERENCE*****************************************************/
#pragma romdata user
uTab UserDynamixelBusModuleTable = {&UserDynamixelBusInit,&UserDynamixelBusRelease,&UserDynamixelBusConfigure,"dynamix"}; //modName must be less or equal 8 characters
#pragma code

/** D E C L A R A T I O N S **************************************************/
#pragma code module

/******************************************************************************
 * Function:        UserDynamixelBusInit(void)
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
 *                    when the module is opened    
 *
 * Note:            None
 *****************************************************************************/

void UserDynamixelBusInit(byte i) {
    BOOL res;
    byte resWriteInfo;
    usrDynamixelBusHandler = i;
    // add my receive function to the handler module, to be called automatically when the pc sends data to the user module
    setHandlerReceiveFunction(usrDynamixelBusHandler,&UserDynamixelBusReceived);
    // add my receive pooling function to the dynamic pooling module, to be called periodically 
    /* andres res = addPollingFunction(&UserDynamixelBusProcessIO);*/
    // initialize the send buffer, used to send data to the PC
    sendBufferUsrDynamixelBus = getSharedBuffer(usrDynamixelBusHandler);
}

/******************************************************************************
/* Function:        UserDynamixelBusConfigure(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        This function sets the specific configuration for the user module, it is called by the framework 
 *                        
 *
 * Note:            None
 *****************************************************************************/
void UserDynamixelBusConfigure(void){
// Do the configuration
}

/******************************************************************************
 * Function:        UserDynamixelBusProcessIO(void)
 *
 * PreCondition:    None
 * 
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        This function is registered in the dinamic polling, who call ir periodically to process the IO interaction
 *                    int the PIC, also it can comunicate things to the pc by the USB    
 *
 * Note:            None
 *****************************************************************************/

void UserDynamixelBusProcessIO(void){

    if((usb_device_state < CONFIGURED_STATE)||(UCONbits.SUSPND==1)) return;
}//end ProcessIO

/******************************************************************************
 * Function:        UserDynamixelBusRelease(byte i)
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
 *                    when the module is close    
 *
 * Note:            None
 *****************************************************************************/

void UserDynamixelBusRelease(byte i) {
    unsetHandlerReceiveBuffer(i);
    unsetHandlerReceiveFunction(i);
    //unregisterT0event(&ax12Event);
    removePoolingFunction(&UserDynamixelBusProcessIO);
}


/******************************************************************************
 * Function:        UserDynamixelBusReceived(byte* recBuffPtr, byte len)
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

void UserDynamixelBusReceived(byte* recBuffPtr, byte len){
      //WORD data_received;
      byte UserDynamixelBusCounter = 0;
      byte dynamixel_packet_length, dynamixel_bus_packet_length;
      byte i;

      switch(((DYNAMIXEL_BUS_DATA_PACKET*)recBuffPtr)->CMD){
        case READ_VERSION:
              ((DYNAMIXEL_DATA_PACKET*)sendBufferUsrDynamixelBus)->_byte[0] = ((DYNAMIXEL_DATA_PACKET*)recBuffPtr)->_byte[0];
              ((DYNAMIXEL_DATA_PACKET*)sendBufferUsrDynamixelBus)->_byte[1] = ((DYNAMIXEL_DATA_PACKET*)recBuffPtr)->_byte[1];
              ((DYNAMIXEL_DATA_PACKET*)sendBufferUsrDynamixelBus)->_byte[2] = AX12_MINOR_VERSION;
              ((DYNAMIXEL_DATA_PACKET*)sendBufferUsrDynamixelBus)->_byte[3] = AX12_MAJOR_VERSION;
              UserDynamixelBusCounter = 0x04;
              break;  
        case SEND:
              ((DYNAMIXEL_DATA_PACKET*)sendBufferUsrDynamixelBus)->_byte[0] = ((DYNAMIXEL_DATA_PACKET*)recBuffPtr)->_byte[0];
              dynamixel_packet_length =  ((DYNAMIXEL_DATA_PACKET*)recBuffPtr)->_byte[4];
              dynamixel_bus_packet_length = dynamixel_packet_length + HEADER_LENGTH + CRC_LENGTH + LENGTH_BYTE;
              setTX();
              for(i=1;i<=dynamixel_bus_packet_length;i++){
                  ax12writeB(((DYNAMIXEL_DATA_PACKET*)recBuffPtr)->_byte[i]);
              }
              setRX();
              UserDynamixelBusCounter = 0x01;
              break;
        case RECEIVE:
              ((DYNAMIXEL_DATA_PACKET*)sendBufferUsrDynamixelBus)->_byte[0] = ((DYNAMIXEL_DATA_PACKET*)recBuffPtr)->_byte[0];
              id       = (byte)(((DYNAMIXEL_DATA_PACKET*)recBuffPtr)->_byte[1]);
              data[0] = PRESENT_POSITION_L;
              data[1] = 0x02; /*length of dTA , data);*/
              ax12SendPacket (id, 0x02, READ_DATA , data);
              value = ax12ReadPacket(&id, &err, &data_received);
              ((DYNAMIXEL_DATA_PACKET*)sendBufferUsrDynamixelBus)->_byte[1] = data_received / 256;
              ((DYNAMIXEL_DATA_PACKET*)sendBufferUsrDynamixelBus)->_byte[2] = data_received % 256;
              UserDynamixelBusCounter = 0x03;
              break;
        case RESET:
              Reset();
              break;
     
         default:
              break;
      }//end switch(s)
      if(UserDynamixelBusCounter != 0){
            j = 255;
            while(mUSBGenTxIsBusy() && j-->0); // pruebo un máximo de 255 veces
                if(!mUSBGenTxIsBusy())
                    USBGenWrite2(usrDynamixelBusHandler, UserDynamixelBusCounter);
      }//end if            
}//end UserDynamixelBusReceived

/** EOF usr_Buzzer.c ***************************************************************/
