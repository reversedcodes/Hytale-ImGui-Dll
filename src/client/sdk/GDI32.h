#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace GDI32
{
    bool Init();

    using tSelectObject = HGDIOBJ (WINAPI*)(HDC, HGDIOBJ);
    using tDeleteObject = BOOL    (WINAPI*)(HGDIOBJ);

    using tSetBkMode    = int     (WINAPI*)(HDC, int);
    using tSetTextColor = COLORREF(WINAPI*)(HDC, COLORREF);
    using tTextOutW     = BOOL    (WINAPI*)(HDC, int, int, LPCWSTR, int);

    using tCreateFontW  = HFONT (WINAPI*)(
        int,int,int,int,int,
        DWORD,DWORD,DWORD,DWORD,
        DWORD,DWORD,DWORD,DWORD,
        LPCWSTR
    );

    using tCreateCompatibleDC = HDC (WINAPI*)(HDC);
    using tDeleteDC           = BOOL (WINAPI*)(HDC);

    using tCreateDIBSection   = HBITMAP (WINAPI*)(
        HDC, const BITMAPINFO*, UINT, VOID**, HANDLE, DWORD
    );

    using tBitBlt = BOOL (WINAPI*)(
        HDC, int, int, int, int,
        HDC, int, int, DWORD
    );

    extern tSelectObject SelectObject;
    extern tDeleteObject DeleteObject;

    extern tSetBkMode SetBkMode;
    extern tSetTextColor SetTextColor;
    extern tTextOutW TextOutW;

    extern tCreateFontW CreateFontW;

    extern tCreateCompatibleDC CreateCompatibleDC;
    extern tDeleteDC DeleteDC;
    extern tCreateDIBSection CreateDIBSection;
    extern tBitBlt BitBlt;
}
