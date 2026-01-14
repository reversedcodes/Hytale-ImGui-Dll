#pragma once
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <cstdint>
#include <cstddef>
#include <string>
#include <vector>
#include <optional>

namespace mem
{

    struct ModuleInfo
    {
        HMODULE handle = nullptr;
        std::uintptr_t base = 0;
        std::size_t size = 0;
    };

    struct PatternByte
    {
        std::uint8_t value = 0;
        bool wildcard = false;
    };

    std::optional<ModuleInfo> GetModuleInfo(const wchar_t *moduleName);

    std::vector<PatternByte> ParsePattern(const std::string &pattern);

    std::optional<std::uintptr_t> FindSignature(
        const ModuleInfo &mod,
        const std::string &pattern,
        bool executableOnly = false);

    bool Protect(void *address, std::size_t size, DWORD newProtect, DWORD *oldProtect);

    template <typename T>
    T Read(std::uintptr_t address)
    {
        return *reinterpret_cast<const T *>(address);
    }

    template <typename T>
    void Write(std::uintptr_t address, const T &value)
    {
        *reinterpret_cast<T *>(address) = value;
    }

    std::uintptr_t ResolveRipRelative(std::uintptr_t instrAddress, std::size_t instrLen, std::ptrdiff_t dispOffset);

}
