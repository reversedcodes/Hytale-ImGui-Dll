#pragma once

#include <windows.h>

class Module {
public:
    virtual ~Module() = default;

    virtual const char* Name() const = 0;
    virtual void OnInitialize() {}
    virtual void OnEnable() {}
    virtual void OnDisable() {}
    virtual void OnShutdown() {}

    virtual void OnUpdate() {}
    virtual void OnSwapBuffers(HDC) {}
    virtual void OnWndProc(HWND, UINT, WPARAM, LPARAM) {}

    bool IsEnabled() const { return m_enabled; }
    void Enable()  { if (!m_enabled) { OnEnable();  m_enabled = true; } }
    void Disable() { if (m_enabled)  { OnDisable(); m_enabled = false; } }

private:
    bool m_enabled = false;
};
