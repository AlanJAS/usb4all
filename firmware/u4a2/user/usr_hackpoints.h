/* Author     Date        Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * Ayle
 *****************************************************************************/

#ifndef HACK_POINTS_H
#define HACK_POINTS_H

/** I N C L U D E S **********************************************************/
#include "system/typedefs.h"
#include "user/adminModule.h"
#include "user/loaderModule.h"

/** D E F I N I T I O N S ****************************************************/

#define HACK_POINTS_MINOR_VERSION   0x02    /*Hackpoints version*/
#define HACK_POINTS_MAJOR_VERSION   0x00

/** S T R U C T U R E S ******************************************************/
enum {
U4A_USR_HACKPOINTS_READ_VERSION = 0x00,
            U4A_USR_HACKPOINTS_SET_MODE = 0x01,
            U4A_USR_HACKPOINTS_READ = 0x02,
            U4A_USR_HACKPOINTS_WRITE = 0x03,
            U4A_USR_HACKPOINTS_SET_PORT = 0x04,
            U4A_USR_HACKPOINTS_SET_PORT_IN = 0x05,
            U4A_USR_HACKPOINTS_SET_PORT_OUT = 0x06
};

typedef union HACK_POINTS_PACKET {
    byte _byte[USBGEN_EP_SIZE]; /* For byte access */
    word _word[USBGEN_EP_SIZE / 2]; /* For word access(USBGEN_EP_SIZE must be even) */

    struct {

        byte CMD;
        byte len;
    };

    struct {
        unsigned char : 8;
        byte ID;
    };

    struct {
        unsigned char : 8;
        byte higth;
        byte low;
    };

    struct {
        unsigned char : 8;
        word word_data;
    };
} HACK_POINTS_DATA_PACKET;

/* Declarations */
#define ZERO 0x00
#define INPUT  0xFF
#define OUTPUT 0x00
#define MASK 0x01

#endif /*HACK_POINTS_H*/
