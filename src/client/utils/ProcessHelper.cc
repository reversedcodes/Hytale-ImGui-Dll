#include "ProcessHelper.h"

#include <Psapi.h>

namespace ProcessHelper
{
    void* getExecutableBase()
    {
        HMODULE hMod = GetModuleHandleA(nullptr);
        return (void*)hMod;
    }

    void* getModuleBase(const char* moduleName)
    {
        HMODULE hMod = GetModuleHandleA(moduleName);
        return (void*)hMod;
    }

    void* getModuleBaseLoad(const char* moduleName)
    {
        HMODULE h = GetModuleHandleA(moduleName);
        if (!h) h = LoadLibraryA(moduleName);
        return (void*)h;
    }

    size_t getModuleSize(void* moduleBase)
    {
        if (!moduleBase) return 0;

        MODULEINFO mi{};
        if (!GetModuleInformation(GetCurrentProcess(), (HMODULE)moduleBase, &mi, sizeof(mi)))
            return 0;

        return (size_t)mi.SizeOfImage;
    }

    void* getExport(void* moduleBase, const char* name)
    {
        if (!moduleBase) return nullptr;
        return (void*)GetProcAddress((HMODULE)moduleBase, name);
    }

    HANDLE getCurrentProcess()
    {
        return ::GetCurrentProcess();
    }

    DWORD getCurrentPID()
    {
        return ::GetCurrentProcessId();
    }
}
