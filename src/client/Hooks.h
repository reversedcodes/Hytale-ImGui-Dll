#pragma once
#include "TFuncHook.h"
#include "modules/ImGuiModule.h"
#include <memory>

class Hooks
{
public:
    static void Init();
    static void Enable();
    static void Restore();

private:
    static inline HWND   g_hWnd = nullptr;
    static inline bool   g_hwndInitialized = false;
    static inline WNDPROC oWndProc = nullptr;

private:
    static BOOL WINAPI hkwglSwapBuffers(HDC hdc);
    static LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
    std::unique_ptr<TFuncHook> m_pWglSwapBuffersHook;
};

extern Hooks g_Hooks;
extern ImGuiModule g_ImGuiModule;