/* Author             									  Date        Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *
 *****************************************************************************/

#ifndef USER_DYNAMIXEL_BUS_H
#define USER_DYNAMIXEL_BUS_H

/** I N C L U D E S **********************************************************/
#include "../system/typedefs.h"
#include "adminModule.h"
#include "loaderModule.h"
#include "ax12.h"


/** D E F I N I T I O N S ****************************************************/

#define DYNAMIXEL_BUS_MINOR_VERSION   0x01    //version 0.1
#define DYNAMIXEL_BUS_MAJOR_VERSION   0x00
 
/** S T R U C T U R E S ******************************************************/
typedef union DYNAMIXEL_BUS_DATA_PACKET{
    byte _byte[USBGEN_EP_SIZE];  //For byte access
    word _word[USBGEN_EP_SIZE/2];//For word access(USBGEN_EP_SIZE msut be even)
    struct
    {
        enum
        { 
            READ_VERSION    = 0x00,
            SEND_BUS        = 0x01,
            RECEIVE_BUS     = 0x02
        } CMD;
        byte len;
    };
    struct
    {
        unsigned :8;
        byte id;
    };
    struct
    {
        unsigned :8;
        byte dynamixel_bus_num;
        byte dynamixel_bus_status;
    };
    struct
    {
        unsigned :8;
        word word_data;
    };
} DYNAMIXEL_BUS_DATA_PACKET;

/** P U B L I C  P R O T O T Y P E S *****************************************/

#endif //USER_DYNAMIXEL_BUS_H
