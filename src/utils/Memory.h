#pragma once
#include <windows.h>
#include <string>
#include <vector>
#include <optional>
#include <cstdint>

namespace mem
{
    struct PatternByte {
        std::uint8_t value;
        bool wildcard;
    };

    struct ModuleInfo {
        HMODULE handle;
        std::uintptr_t base;
        std::size_t size;
    };

    std::uintptr_t FindPattern(const std::string& pattern, const wchar_t* moduleName = nullptr);
    
    std::optional<ModuleInfo> GetModuleInfo(const wchar_t* moduleName);
    std::optional<std::uintptr_t> FindSignature(const ModuleInfo& mod, const std::string& pattern, bool executableOnly = true);
    std::uintptr_t ResolveRipRelative(std::uintptr_t instrAddress, std::size_t instrLen, std::ptrdiff_t dispOffset);
}