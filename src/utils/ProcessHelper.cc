#include "ProcessHelper.h"

#ifdef _WIN32
#include <Windows.h>
#include <Psapi.h>
#endif

namespace ProcessHelper {
    void *getExecutableBase() {
        MODULEINFO mi = {nullptr};
        GetModuleInformation(GetCurrentProcess(), GetModuleHandleA(nullptr), &mi, sizeof(mi));
        return mi.lpBaseOfDll;
    }
}