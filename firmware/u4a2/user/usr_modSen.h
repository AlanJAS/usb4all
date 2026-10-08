/* Author                                           Date        Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * Alan Aguiar                                      01/08/123   Original.
 *****************************************************************************/

#ifndef USER_MODSEN_H
#define USER_MODSEN_H

/** I N C L U D E S **********************************************************/
#include "system/typedefs.h"
#include "user/adminModule.h"
#include "user/loaderModule.h"

/** D E F I N I T I O N S ****************************************************/

#define MODSEN_MINOR_VERSION   0x01
#define MODSEN_MAJOR_VERSION   0x00

/** S T R U C T U R E S ******************************************************/
enum {
U4A_USR_MODSEN_READ_VERSION = 0x00,
            U4A_USR_MODSEN_GET_VALUE    = 0x01,
            U4A_USR_MODSEN_RESET	 = 0xFF /*backward compatibility*/
};

typedef union MODSEN_DATA_PACKET
{
    byte _byte[USBGEN_EP_SIZE];  /*For byte access*/
    word _word[USBGEN_EP_SIZE/2];/*For word access(USBGEN_EP_SIZE msut be even)*/
    struct
    {
        byte CMD;
        byte len;
    };
    struct
    {
        unsigned char :8;
        byte ID;
    };
    struct
    {
        unsigned char :8;
        byte modulea_num;
        byte modulea_status;
    };
    struct
    {
        unsigned char :8;
        word word_data;
    };
} MODSEN_DATA_PACKET;

/** P U B L I C  P R O T O T Y P E S *****************************************/

#endif /*USER_MODSEN_H*/
