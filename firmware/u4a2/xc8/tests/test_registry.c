/* Host-only fixtures: test the real registry without emulating PIC registers. */
#include <assert.h>
#include <string.h>
#include "user/module_registry.h"
static byte seen;
static void init(byte handler) { seen = handler; }
static void release(byte handler) { seen = handler + 1; }
#define TABLE(symbol, name) const uTab symbol = {init, release, name}
TABLE(AdminModuleTable, "admin");
TABLE(PNPModuleTable, "pnp");
TABLE(PortModuleTable, "port");
TABLE(userAX12ModuleTable, "ax");
TABLE(userButtonModuleTable, "button");
TABLE(HackPointsModuleTable, "hackp");
TABLE(userMotorsModuleTable, "motors");
TABLE(UserButiaModuleTable, "butia");
TABLE(userModActATable, "modActA");
TABLE(userModActBTable, "modActB");
TABLE(userModActCTable, "modActC");
TABLE(userRelayModTable, "relay");
TABLE(userModSenATable, "modSenA");
TABLE(userModSenBTable, "modSenB");
TABLE(userModSenCTable, "modSenC");
TABLE(userGreyModuleTable, "grey");
TABLE(userLightModuleTable, "light");
TABLE(userResModuleTable, "res");
TABLE(userVoltModuleTable, "volt");
TABLE(userDistModuleTable, "distanc");
int main(void)
{
    static const char expected[][8] = {
        "admin", "pnp", "port", "ax", "button", "hackp", "motors", "butia",
        "modActA", "modActB", "modActC", "relay", "modSenA", "modSenB",
        "modSenC", "grey", "light", "res", "volt", "distanc"
    };
    char name[8];
    byte id[8], i;
    const uTab *table;
    assert(getUserTableSize() == 20);
    for (i = 0; i < 20; ++i) {
        getModuleName(i, name);
        assert(memcmp(name, expected[i], 8) == 0);
        memcpy(id, name, 8);
        table = getUserTableDirection(id);
        assert(table != 0 && getModuleType(table) == i);
        getModuleInitDirection(table)(42);
        assert(seen == 42);
        getModuleReleaseDirection(table)(42);
        assert(seen == 43);
    }
    memset(id, 'x', sizeof id); /* Eight bytes, no terminating NUL. */
    assert(getUserTableDirection(id) == 0);
    memset(id, 0, sizeof id);
    assert(getUserTableDirection(id) == 0);
    memcpy(id, "adm", 4); /* Prefix must not match admin. */
    assert(getUserTableDirection(id) == 0);
    assert(getModuleType(0) == NULLTYPE);
    assert(getModuleInitDirection(0) == 0);
    assert(getModuleReleaseDirection(0) == 0);
    getModuleName(255, name);
    for (i = 0; i < 8; ++i) assert(name[i] == 0);
    return 0;
}
