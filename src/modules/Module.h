#pragma once

class Module {
    public:

        virtual ~Module() = default;

        virtual void OnInitialize() = 0;
        virtual void OnEnable() = 0;
        virtual void OnDisable() = 0;

        virtual bool IsEnabled() const {
            return m_enabled;
        }

        virtual void Enable() {
            if (m_enabled)
                return;
            OnEnable();
            m_enabled = true;
        }

        virtual void Disable() {
            if (!m_enabled)
                return;
            OnDisable();
            m_enabled = false;
        }

    private:
        bool m_enabled = false;
};