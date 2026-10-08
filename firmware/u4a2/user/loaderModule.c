/* USB4all module registry for XC8. No assumptions about function-pointer size. */
#include "loaderModule.h"
#include "module_registry.h"

#define MODULE_ADDRESS(name) &name,
static const uTab * const modules[] = { U4A_MODULES(MODULE_ADDRESS) };
#undef MODULE_ADDRESS
#define MODULE_COUNT (sizeof modules / sizeof modules[0])
typedef char module_count_fits_byte[(MODULE_COUNT < NULLTYPE) ? 1 : -1];

const uTab *getUserTableDirection(byte moduleId[8])
{
    byte i, j;
    for (i = 0; i < MODULE_COUNT; ++i) {
        for (j = 0; j < 8; ++j) {
            if ((byte)moduleId[j] != modules[i]->id[j]) break;
            if (moduleId[j] == 0) return modules[i];
        }
        if (j == 8) return modules[i];
    }
    return (const uTab *)0;
}

byte getUserTableSize(void) { return (byte)MODULE_COUNT; }

byte getModuleType(const uTab *direction)
{
    byte i;
    for (i = 0; i < MODULE_COUNT; ++i) {
        if (direction == modules[i]) return i;
    }
    return NULLTYPE;
}

void getModuleName(byte line, char *name)
{
    byte j;
    for (j = 0; j < 8; ++j)
        name[j] = line < MODULE_COUNT ? modules[line]->id[j] : 0;
}

pUserFunc getModuleInitDirection(const uTab *direction)
{
    byte index = getModuleType(direction);
    return index == NULLTYPE ? (pUserFunc)0 : modules[index]->pfI;
}

pUserFunc getModuleReleaseDirection(const uTab *direction)
{
    byte index = getModuleType(direction);
    return index == NULLTYPE ? (pUserFunc)0 : modules[index]->pfR;
}
