#pragma once
#include <Windows.h>
#include <cstddef>

namespace ProcessHelper
{
    void*  getExecutableBase();
    void*  getModuleBase(const char* moduleName);
    void*  getModuleBaseLoad(const char* moduleName);
    size_t getModuleSize(void* moduleBase);
    void*  getExport(void* moduleBase, const char* name);

    HANDLE getCurrentProcess();
    DWORD getCurrentPID();
}
