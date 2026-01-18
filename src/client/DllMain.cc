#include "Hooks.h"
#include "utils/Console.h"
#include "utils/ProcessHelper.h"

#include <iostream>

static DWORD WINAPI init(LPVOID hModule)
{
    CreateConsole();
    Hooks::Init();
    Hooks::Enable();

    std::cout << "Base Address: " << ProcessHelper::getExecutableBase() << std::endl;

    return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID)
{
    if (reason == DLL_PROCESS_ATTACH)
    {
        DisableThreadLibraryCalls(hModule);
        CreateThread(nullptr, 0, init, hModule, 0, nullptr);
    }
    else if (reason == DLL_PROCESS_DETACH)
    {
        Hooks::Restore();
    }
    return TRUE;
}