/* Author               Date        Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * Andres Aguirre	   27/03/07 	Original
 * Andres Aguirre 	   18/04/09     adding U4A_ADMINMODULE_RESET
 * Andres Aguirre          22/03/12     Adding PnP support
 * Alan Aguiar             31/07/13     Lot of changes - cleans
 ********************************************************************/

#include "user/adminModule.h"
#include "system/xc8_eeprom.h"
#include "system/xc8_clock.h"
#include "user/pnp.h"
#include "user/usb4butia.h"
#include "handlerManager.h"

/** V A R I A B L E S ********************************************************/
byte* sendBufferAdmin;
byte adminHandler;


/** USER MODULE REFERENCE *************************************************/
const uTab AdminModuleTable = {&adminModuleInit, &adminModuleRelease, "admin"}; //modName must be less or equal 8 characters


/** P R I V A T E  P R O T O T Y P E S ***************************************/

/** D E C L A R A T I O N S **************************************************/

void Busy_eep_non_block() {
    byte j = 255;
    while (EECON1bits.WR && j-- > (byte) 0);
}

void Escribir_memoria_boot(void) {
    Busy_eep_non_block();
    u4a_eeprom_write(ADDRESS_BOOT, BOOT_FLAG);
}

void adminModuleInit(byte handler) {
    /*system initialization*/
    adminHandler = handler; //hardcode, the admin module allways respond at handler 0
    /*set the receive function for admin commands*/
    setHandlerReceiveFunction(adminHandler, &adminReceived);
    sendBufferAdmin = getSharedBuffer(adminHandler);
}

void adminModuleRelease(byte handler) {
    /*what? close admin? Are you crazy?*/
    return;
}

void goodByeCruelWorld(void) {
    //When resetting, make sure to drop the device off the bus
    //for a period of time. Helps when the device is suspended.
    UCONbits.USBEN = 0;
    __delay_ms(100); /* Must not be optimized away before reconnecting USB. */
    Reset();
}

void adminReceived(byte* recBuffPtr, byte len, byte admin_handler) {
    byte adminCounter;
    byte endIn = nullEP;
    byte lineNumber = 0;
    char lineName[8];
    const uTab *tableDirec;
    void (*pUser)(byte);
    byte handler, response;
    byte j;
    adminCounter = 0;

    switch (((AM_PACKET*) recBuffPtr)->CMD) {
        case U4A_ADMINMODULE_GET_FIRMWARE_VERSION:
            ((AM_PACKET*) sendBufferAdmin)->CMD = U4A_ADMINMODULE_GET_FIRMWARE_VERSION;
            ((AM_PACKET*) sendBufferAdmin)->size = FIRMWARE_VERSION;
            adminCounter = 0x02;
            break;

            /* Abre un user module, y retorna el handler asignado en el sistema*/
        case U4A_ADMINMODULE_OPEN:
            tableDirec = getUserTableDirection(((AM_PACKET*) recBuffPtr)->moduleId);
            if(!existsTableEntry(tableDirec)){
                if (tableDirec != (const uTab *)0) {
                    endIn = ((AM_PACKET*) recBuffPtr)->inEp;
                    handler = newHandlerTableEntry(endIn, tableDirec);
                    pUser = getModuleInitDirection(tableDirec);
                    pUser(handler); //hago el init ;)
                    ((AM_PACKET*) sendBufferAdmin)->handlerNumber = handler;
                } else {
                    ((AM_PACKET*) sendBufferAdmin)->handlerNumber = ERROR;
                }
            } else{
                ((AM_PACKET*)sendBufferAdmin)->handlerNumber = handlerFromTableEntry(tableDirec);
            }
            ((AM_PACKET*) sendBufferAdmin)->CMD = U4A_ADMINMODULE_OPEN;
            adminCounter = 0x02; //1 byte para el campo CMD, otro para el handler
            break;

            /* Cierra un user module */
        case U4A_ADMINMODULE_CLOSE:
            handler = ((AM_PACKET*) recBuffPtr)->handlerNumber;
            response = removeHandlerTableEntry(handler);
            ((AM_PACKET*) sendBufferAdmin)->response = response;
            ((AM_PACKET*) sendBufferAdmin)->CMD = U4A_ADMINMODULE_CLOSE;
            adminCounter = 0x02; //1 byte para el campo CMD, otro para la respuesta
            break;

            /* Cierra todos los modulos */
        case U4A_ADMINMODULE_INIT:
            //removeAllOpenModules(); called by bobot, at start, dont work whit autodetection!!!
            ((AM_PACKET*) sendBufferAdmin)->CMD = U4A_ADMINMODULE_INIT;
            adminCounter = 0x01; //1 byte para el campo CMD
            break;

        case U4A_ADMINMODULE_MESSAGE:
            // me limito a solamente mandar el paquete que me genera el usuario
            for (adminCounter = 0; adminCounter < len; adminCounter++) {
                *(sendBufferAdmin + adminCounter) = *(recBuffPtr + adminCounter);
            }
            adminCounter = len;
            break;

        case U4A_ADMINMODULE_LOAD:
            //((AM_PACKET*) sendBufferAdmin)->CMD = U4A_ADMINMODULE_LOAD;
            //Ver loaderModule.h
            //loadModule(byte idModule, byte* binaryStream);
            //adminCounter = 0x01;
            break;

        case U4A_ADMINMODULE_UNLOAD:
            //((AM_PACKET*) sendBufferAdmin)->CMD = U4A_ADMINMODULE_UNLOAD;
            //adminCounter = 0x01;
            break;

            /* retorna la cantidad de modulos de usuarios presentes en el firmware */
        case U4A_ADMINMODULE_GET_USER_MODULES_SIZE:
            ((AM_PACKET*) sendBufferAdmin)->CMD = U4A_ADMINMODULE_GET_USER_MODULES_SIZE;
            ((AM_PACKET*) sendBufferAdmin)->size = getUserTableSize();
            adminCounter = 0x02;
            break;

            /* retorna el nombre correspondiente al modulo de usuario n-esimo*/
        case U4A_ADMINMODULE_GET_USER_MODULES_LINE:
            ((AM_PACKET*) sendBufferAdmin)->CMD = U4A_ADMINMODULE_GET_USER_MODULES_LINE;
            lineNumber = ((AM_PACKET*) recBuffPtr)->line;
            getModuleName(lineNumber, (char*) lineName);
            for (j = (byte) 0; j < (byte) 8; j++) {
                ((AM_PACKET*) sendBufferAdmin)->lineName[j] = lineName[j];
            }
            adminCounter = 0x09;
            break;

        case U4A_ADMINMODULE_BOOT:
            Escribir_memoria_boot();
            goodByeCruelWorld();
            break;

        case U4A_ADMINMODULE_GET_HANDLER_SIZE:
            ((AM_PACKET*) sendBufferAdmin)->CMD = U4A_ADMINMODULE_GET_HANDLER_SIZE;
            ((AM_PACKET*) sendBufferAdmin)->size = getMaxHandler();
            adminCounter = 0x02;
            break;

        case U4A_ADMINMODULE_GET_HANDLER_TYPE:
            ((AM_PACKET*) sendBufferAdmin)->CMD = U4A_ADMINMODULE_GET_HANDLER_TYPE;
            handler = ((AM_PACKET*) recBuffPtr)->size;
            handler = handler % MAX_HANDLERS; //sanity check
            if (epHandlerMap[handler].ep.empty == (unsigned) 0) {
                ((AM_PACKET*) sendBufferAdmin)->type = getModuleType(epHandlerMap[handler].uTableDirection);
            } else {
                ((AM_PACKET*) sendBufferAdmin)->type = NULLTYPE;
            };
            adminCounter = 0x02;
            break;

        case U4A_ADMINMODULE_RESET:
            goodByeCruelWorld();
            break;

        default:
            break;

    }//end switch()

    USBGenWrite2(adminHandler, adminCounter);

}//end adminReceived


/** EOF adminModule.c ***************************************************************/


