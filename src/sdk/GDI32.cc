#include "GDI32.h"

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

    bool Init()
    {
        HMODULE mod = GetModuleHandleW(L"gdi32.dll");
        
        if (!mod) return false;

        SelectObject = (tSelectObject)GetProcAddress(mod, "SelectObject");
        DeleteObject = (tDeleteObject)GetProcAddress(mod, "DeleteObject");

        SetBkMode    = (tSetBkMode)GetProcAddress(mod, "SetBkMode");
        SetTextColor = (tSetTextColor)GetProcAddress(mod, "SetTextColor");
        TextOutW     = (tTextOutW)GetProcAddress(mod, "TextOutW");

        CreateFontW  = (tCreateFontW)GetProcAddress(mod, "CreateFontW");

        CreateCompatibleDC = (tCreateCompatibleDC)GetProcAddress(mod, "CreateCompatibleDC");
        DeleteDC           = (tDeleteDC)GetProcAddress(mod, "DeleteDC");
        CreateDIBSection   = (tCreateDIBSection)GetProcAddress(mod, "CreateDIBSection");
        BitBlt             = (tBitBlt)GetProcAddress(mod, "BitBlt");

        return SelectObject && DeleteObject && SetBkMode && SetTextColor && TextOutW && CreateFontW && CreateCompatibleDC && DeleteDC && CreateDIBSection && BitBlt;
    }
}
