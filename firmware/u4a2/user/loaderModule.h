/* Author               Date        Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * Andres Aguirre	   03/04/07		Original
 *****************************************************************************/

#ifndef LOADER_MODULE_H
#define LOADER_MODULE_H

/** I N C L U D E S **********************************************************/
#include "system/typedefs.h"
/** D E F I N I T I O N S ****************************************************/
#define NULLTYPE 0xFF

/** S T R U C T U R E S ******************************************************/

typedef void(*pUserFunc)(byte);  // defino el tipo que representa las funciones init del usuario

typedef struct uTab{         // struct para mapear en const los datos que identifican a las funciones init del usuario
	pUserFunc pfI;				// puntero a la funcion Init del usuario
	pUserFunc pfR;				// puntero a la funcion Release del usuario
	byte id[8];				// identificador del modulo usuario
} uTab;


/** P U B L I C  P R O T O T Y P E S *****************************************/

// Para cargar el modulo se le pasa el binaryStream que es el .hex a cargar
// y un identificador de modulo
//void loadModule(byte idModule, byte* binaryStream);
const uTab *getUserTableDirection(byte moduleId[8]);
byte getUserTableSize(void);
void getModuleName(byte line, char* modName);
pUserFunc getModuleInitDirection(const uTab *direction);
pUserFunc getModuleReleaseDirection(const uTab *direction);
byte getModuleType(const uTab *uTableDirection);
#endif //LOADER_MODULE_H
