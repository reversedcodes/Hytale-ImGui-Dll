#include "GDI32.h"
#include "Utils/ProcessHelper.h"

#include <Windows.h>

namespace GDI32
{
    tSelectObject SelectObject = nullptr;
    tDeleteObject DeleteObject = nullptr;

    tSetBkMode SetBkMode = nullptr;
    tSetTextColor SetTextColor = nullptr;
    tTextOutW TextOutW = nullptr;

    tCreateFontW CreateFontW = nullptr;

    tCreateCompatibleDC CreateCompatibleDC = nullptr;
    tDeleteDC DeleteDC = nullptr;
    tCreateDIBSection CreateDIBSection = nullptr;
    tBitBlt BitBlt = nullptr;

    static FARPROC GetProc(HMODULE mod, const char* name)
    {
        if (!mod) return nullptr;
        return ::GetProcAddress(mod, name);
    }

    bool Init()
    {
        HMODULE mod = (HMODULE)ProcessHelper::getModuleBase("gdi32.dll");

        SelectObject = reinterpret_cast<tSelectObject>(GetProc(mod, "SelectObject"));
        DeleteObject = reinterpret_cast<tDeleteObject>(GetProc(mod, "DeleteObject"));

        SetBkMode    = reinterpret_cast<tSetBkMode>(GetProc(mod, "SetBkMode"));
        SetTextColor = reinterpret_cast<tSetTextColor>(GetProc(mod, "SetTextColor"));
        TextOutW     = reinterpret_cast<tTextOutW>(GetProc(mod, "TextOutW"));

        CreateFontW  = reinterpret_cast<tCreateFontW>(GetProc(mod, "CreateFontW"));

        CreateCompatibleDC = reinterpret_cast<tCreateCompatibleDC>(GetProc(mod, "CreateCompatibleDC"));
        DeleteDC           = reinterpret_cast<tDeleteDC>(GetProc(mod, "DeleteDC"));
        CreateDIBSection   = reinterpret_cast<tCreateDIBSection>(GetProc(mod, "CreateDIBSection"));
        BitBlt             = reinterpret_cast<tBitBlt>(GetProc(mod, "BitBlt"));

        return SelectObject && DeleteObject && SetBkMode && SetTextColor && TextOutW && CreateFontW && CreateCompatibleDC && DeleteDC && CreateDIBSection && BitBlt;
    }
}
