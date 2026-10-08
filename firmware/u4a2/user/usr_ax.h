/******************************************************************************
 * Author - Date - Comment
 *----------------------------------------------------------------------------
 * Ayle - 02/2013 - new module.
 * Ayle - 02/2013 - fixed read_info function.
 ******************************************************************************/

#ifndef USER_AX_H
#define USER_AX_H

/** I N C L U D E S **********************************************************/
#include "system/typedefs.h"
#include "user/adminModule.h"
#include "user/loaderModule.h"
#include "user/ax12.h"

/** D E F I N I T I O N S ****************************************************/
#define AX_MINOR_VERSION   0x02
#define AX_MAJOR_VERSION   0x00

/** S T R U C T U R E S ******************************************************/
enum {
U4A_USR_AX_READ_VERSION = 0x00,
            U4A_USR_AX_WRITE_INFO = 0x01,
            U4A_USR_AX_READ_INFO = 0x02,
            U4A_USR_AX_SEND_RAW = 0x03,
            U4A_USR_AX_RESET = 0xFF
};

typedef union AX_DATA_PACKET {
    byte _byte[USBGEN_EP_SIZE];
    word _word[USBGEN_EP_SIZE / 2];

    struct {

        byte CMD;
        byte len;
    };

    struct {
        unsigned char : 8;
        byte id;
    };

    struct {
        unsigned char : 8;
        byte ax12_num;
        byte ax12_status;
    };

    struct {
        unsigned char : 8;
        word word_data;
    };
} AX_DATA_PACKET;

/** P U B L I C  P R O T O T Y P E S *****************************************/

#endif /* USR_AX_H */
