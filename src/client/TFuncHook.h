#include <funchook.h>
#include <stdexcept>
#include <string>

class TFuncHook
{
public:
    void *funcPtr = nullptr;
    void *funcReal = nullptr;

    TFuncHook() = default;

    TFuncHook(void *func, void *hooked)
    {
        Init(func, hooked);
    }

    ~TFuncHook()
    {
        Restore();
    }

    TFuncHook(const TFuncHook &) = delete;
    TFuncHook &operator=(const TFuncHook &) = delete;

    void Enable()
    {
        if (!m_hook)
            throw std::runtime_error("FuncHook::Enable: hook not initialized");
        if (m_installed)
            return;

        int st = funchook_install(m_hook, 0);
        if (st != 0)
            throw std::runtime_error("funchook_install failed: " + std::to_string(st));
        m_installed = true;
    }

    void Restore()
    {
        if (!m_hook)
            return;

        if (m_installed)
        {
            funchook_uninstall(m_hook, 0);
            m_installed = false;
        }

        funchook_destroy(m_hook);

        m_hook = nullptr;
        funcPtr = nullptr;
        funcReal = nullptr;
        m_detour = nullptr;
    }

    template <typename Fn>
    Fn OriginalAs() const
    {
        return reinterpret_cast<Fn>(funcReal);
    }

    template <typename TRet, typename... TArgs>
    TRet CallOriginal(TArgs... args)
    {
        using FnType = TRet(__thiscall *)(TArgs...);
        auto fn = reinterpret_cast<FnType>(funcReal);
        return fn(args...);
    }

private:
    void Init(void *func, void *hooked)
    {
        funcPtr = func;
        funcReal = func;
        m_detour = hooked;

        m_hook = funchook_create();

        if (!m_hook)
            throw std::runtime_error("funchook_create failed");

        int st = funchook_prepare(m_hook, &funcReal, m_detour);

        if (st != 0)
        {
            funchook_destroy(m_hook);
            m_hook = nullptr;
            throw std::runtime_error("funchook_prepare failed: " + std::to_string(st));
        }
    }

private:
    funchook_t *m_hook = nullptr;
    void *m_detour = nullptr;
    bool m_installed = false;
};
