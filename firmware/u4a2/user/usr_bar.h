/* Author                                           Date        Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * Enrique Madruga                                 31/10/13    Original.
 *****************************************************************************/

#ifndef USER_BAR_H
#define USER_BAR_H

/** I N C L U D E S **********************************************************/
#include "system/typedefs.h"
#include "user/adminModule.h"
#include "user/loaderModule.h"

/** D E F I N I T I O N S ****************************************************/

#define BAR_MINOR_VERSION   0x01    /*Bar Version */
#define BAR_MAJOR_VERSION   0x00

/** S T R U C T U R E S ******************************************************/
typedef union BAR_DATA_PACKET
{
    byte _byte[USBGEN_EP_SIZE];  /*For byte access*/
    word _word[USBGEN_EP_SIZE/2];/*For word access(USBGEN_EP_SIZE msut be even)*/
    struct
    {
        enum
        {
            READ_VERSION = 0x00,
            GET_VALUE    = 0x01,
            IS_COUNTING  = 0x02,
            IS_DONE      = 0x03,
            START        = 0x04,
            GET_TIME     = 0x05,
            RESET	 = 0xFF /*backward compatibility*/
        }CMD;
        byte len;
    };
    struct
    {
        unsigned :8;
        byte ID;
    };
    struct
    {
        unsigned :8;
        byte bar_num;
        byte bar_status;
    };
    struct
    {
        unsigned :8;
        word word_data;
    };
} BAR_DATA_PACKET;

/** P U B L I C  P R O T O T Y P E S *****************************************/

#endif /*USER_BAR_H*/
