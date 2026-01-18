#include "ImGuiModule.h"

#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_win32.h>
#include <windows.h>

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
ImGuiModule g_ImGuiModule;

void ImGuiModule::OnInitialize()
{

}

void ImGuiModule::OnEnable()
{

}

bool ImGuiModule::InitImGuiForCurrentContext(HDC hdc)
{
    if (m_imguiInitialized)
        return true;

    if (!m_hWnd)
        m_hWnd = WindowFromDC(hdc);

    if (!m_hWnd)
        return false;

    HGLRC ctx = wglGetCurrentContext();
    
    if (!ctx)
        return false;

    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    if (!ImGui_ImplWin32_Init(m_hWnd))
        return false;

    if (!ImGui_ImplOpenGL3_Init("#version 130"))
        return false;

    m_imguiInitialized = true;
    return true;
}

void ImGuiModule::OnDisable()
{
    if (m_imguiInitialized)
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplWin32_Shutdown();
        ImGui::DestroyContext();
        m_imguiInitialized = false;
    }
}

void ImGuiModule::OnSwapBuffers(HDC hdc)
{
    if (!m_imguiInitialized)
    {
        if (!InitImGuiForCurrentContext(hdc))
            return;
    }

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("Hello, ImGui!");
    ImGui::Text("This is a sample ImGui window.");
    ImGui::End();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}


void ImGuiModule::OnWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (m_imguiInitialized)
    {
        ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam);
    }
}