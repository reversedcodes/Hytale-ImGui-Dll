#include "Hooks.h"

#include <cstdio>
#include <ranges>
#include <cstdarg>
#include <cmath>
#include <iostream>

#include "common/imgui/imgui.h"
#include "common/imgui/backends/imgui_impl_win32.h"
#include "common/imgui/backends/imgui_impl_opengl3.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

Hooks g_Hooks;
static bool g_ImGuiInitialized = false;
static bool my_tool_active = true;
static float my_color[4] = {0.45f, 0.55f, 0.60f, 1.00f};

WNDPROC oWndProc = nullptr;

void Hooks::Init()
{
    g_Hooks.m_pWglSwapBuffersHook = std::make_unique<FuncHook>(reinterpret_cast<void *>(GetProcAddress(GetModuleHandleA("opengl32.dll"), "wglSwapBuffers")), Hooks::hkwglSwapBuffers);

    HWND hTarget = FindWindowA(nullptr, "Hytale");
    if (hTarget)
    {
        oWndProc = (WNDPROC)SetWindowLongPtrW(hTarget, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(Hooks::WndProc));
    }
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

    HWND hTarget = FindWindowA(nullptr, "Hytale");

    if (hTarget && oWndProc)
    {
        SetWindowLongPtrW(hTarget, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(oWndProc));
    }
}

LRESULT CALLBACK Hooks::WndProc(const HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (g_ImGuiInitialized && my_tool_active)
    {
        if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
            return true;
    }

    if (my_tool_active && g_ImGuiInitialized)
    {
        switch (msg)
        {
        case WM_LBUTTONDOWN:
        case WM_LBUTTONUP:
        case WM_RBUTTONDOWN:
        case WM_RBUTTONUP:
        case WM_MOUSEMOVE:
            return 0;
        }
    }

    return CallWindowProc(oWndProc, hWnd, msg, wParam, lParam);
}
BOOL WINAPI Hooks::hkwglSwapBuffers(HDC hdc)
{
    if (!g_ImGuiInitialized)
    {
        HWND hwnd = WindowFromDC(hdc);
        if (hwnd)
        {
            IMGUI_CHECKVERSION();
            ImGui::CreateContext();
            ImGui_ImplWin32_Init(hwnd);
            ImGui_ImplOpenGL3_Init("#version 130");
            g_ImGuiInitialized = true;
        }
    }

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    if (GetAsyncKeyState(VK_INSERT) & 1)
        my_tool_active = !my_tool_active;

    if (my_tool_active)
    {
        ImGui::Begin("My First Tool", &my_tool_active, ImGuiWindowFlags_MenuBar);

        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("Open..", "Ctrl+O"))
                {
                }
                if (ImGui::MenuItem("Save", "Ctrl+S"))
                {
                }
                if (ImGui::MenuItem("Close", "Ctrl+W"))
                {
                    my_tool_active = false;
                }
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }

        ImGui::ColorEdit4("Color", my_color);

        float samples[100];
        for (int n = 0; n < 100; n++)
            samples[n] = sinf(n * 0.2f + (float)ImGui::GetTime() * 1.5f);

        ImGui::PlotLines("Samples", samples, 100);

        ImGui::TextColored(ImVec4(1, 1, 0, 1), "Important Stuff");

        ImGui::BeginChild("Scrolling", ImVec2(0, 150), true);
        ImGui::Text("%04d: Hello, world!", 1);
        ImGui::EndChild();

        ImGui::End();
    }

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    return g_Hooks.m_pWglSwapBuffersHook->CallOriginal<BOOL, HDC>(hdc);
}