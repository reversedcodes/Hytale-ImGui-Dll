#include "Hooks.h"
#include "utils/Console.h"

static DWORD WINAPI init(LPVOID hModule)
{
    CreateConsole();
    Hooks::Init();
    Hooks::Enable();
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