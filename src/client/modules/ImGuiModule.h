#pragma once
#include "Module.h"
#include <Windows.h>

class ImGuiModule final : public Module
{
public:
    void OnInitialize() override;
    void OnEnable() override;
    void OnDisable() override;

    // called from hook
    void OnSwapBuffers(HDC hdc);

    // called from hook
    void OnWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

    bool EnsureAttachedToWindowFromDC(HDC hdc);

    WNDPROC GetOriginalWndProc() const
    {
        return m_oWndProc;
    }
    
    HWND GetHwnd() const
    {
        return m_hWnd;
    }

private:
    bool InitImGuiForCurrentContext(HDC hdc);

private:
    WNDPROC m_oWndProc = nullptr;
    HWND m_hWnd = nullptr;

    HGLRC m_glrc = nullptr;
    bool m_imguiInitialized = false;
    bool m_attachedWndProc = false;
};
