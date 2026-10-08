#ifndef U4A_MODULE_REGISTRY_H
#define U4A_MODULE_REGISTRY_H
#include "loaderModule.h"
/* Ordered explicitly; keep existing entries in place when adding modules.
 * Module type is the index in this list (not a persistent C18 flash address). */
extern const uTab AdminModuleTable;
extern const uTab PNPModuleTable;
extern const uTab PortModuleTable;
extern const uTab userAX12ModuleTable;
extern const uTab userButtonModuleTable;
extern const uTab HackPointsModuleTable;
extern const uTab userMotorsModuleTable;
extern const uTab UserButiaModuleTable;
extern const uTab userModActATable;
extern const uTab userModActBTable;
extern const uTab userModActCTable;
extern const uTab userRelayModTable;
extern const uTab userModSenATable;
extern const uTab userModSenBTable;
extern const uTab userModSenCTable;
extern const uTab userGreyModuleTable;
extern const uTab userLightModuleTable;
extern const uTab userResModuleTable;
extern const uTab userVoltModuleTable;
extern const uTab userDistModuleTable;

#define U4A_MODULES(X) \
    X(AdminModuleTable) \
    X(PNPModuleTable) \
    X(PortModuleTable) \
    X(userAX12ModuleTable) \
    X(userButtonModuleTable) \
    X(HackPointsModuleTable) \
    X(userMotorsModuleTable) \
    X(UserButiaModuleTable) \
    X(userModActATable) \
    X(userModActBTable) \
    X(userModActCTable) \
    X(userRelayModTable) \
    X(userModSenATable) \
    X(userModSenBTable) \
    X(userModSenCTable) \
    X(userGreyModuleTable) \
    X(userLightModuleTable) \
    X(userResModuleTable) \
    X(userVoltModuleTable) \
    X(userDistModuleTable)
#endif
