#include "Hooks.h"

#include "utils/ProcessHelper.h"
#include "utils/Memory.h"

Hooks g_Hooks;

void Hooks::Init()
{
    auto mod = ProcessHelper::getModuleBaseLoad("opengl32.dll");
    auto target = ProcessHelper::getExport(mod, "wglSwapBuffers");
    if (!target) return;

    g_Hooks.m_pWglSwapBuffersHook = std::make_unique<TFuncHook>(target, Hooks::hkwglSwapBuffers);
}

void Hooks::Enable()
{
    if (g_Hooks.m_pWglSwapBuffersHook)
        g_Hooks.m_pWglSwapBuffersHook->Enable();
}

void Hooks::Restore()
{
    if (g_Hooks.m_pWglSwapBuffersHook)
        g_Hooks.m_pWglSwapBuffersHook->Restore();

    if (g_hWnd && oWndProc)
        SetWindowLongPtrW(g_hWnd, GWLP_WNDPROC, (LONG_PTR)oWndProc);

    oWndProc = nullptr;
    g_hWnd = nullptr;
    g_hwndInitialized = false;
}

LRESULT CALLBACK Hooks::WndProc(const HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{

    if(g_ImGuiModule.IsEnabled())
        g_ImGuiModule.OnWndProc(hWnd, msg, wParam, lParam);

    return CallWindowProc(oWndProc, hWnd, msg, wParam, lParam);
}

BOOL WINAPI Hooks::hkwglSwapBuffers(HDC hdc)
{   
    if(g_hwndInitialized == false) {
        g_hWnd = WindowFromDC(hdc);
        g_hwndInitialized = true;
    }

    if(g_hwndInitialized) {
        if(oWndProc == nullptr) {
            oWndProc = (WNDPROC)GetWindowLongPtrW(g_hWnd, GWLP_WNDPROC);
            SetWindowLongPtrW(g_hWnd, GWLP_WNDPROC, (LONG_PTR)Hooks::WndProc);
        }
    }

    if (g_ImGuiModule.IsEnabled())
        g_ImGuiModule.OnSwapBuffers(hdc);

    return g_Hooks.m_pWglSwapBuffersHook->CallOriginal<BOOL, HDC>(hdc);
}

