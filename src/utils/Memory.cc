#include "Memory.h"

#define WIN32_LEAN_AND_MEAN
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#include <psapi.h>

#include <sstream>
#include <algorithm>

namespace mem
{

    static bool IsReadable(DWORD protect)
    {
        protect &= 0xFF;
        return protect == PAGE_READONLY || protect == PAGE_READWRITE || protect == PAGE_WRITECOPY || protect == PAGE_EXECUTE_READ || protect == PAGE_EXECUTE_READWRITE || protect == PAGE_EXECUTE_WRITECOPY;
    }

    static bool IsExecutable(DWORD protect)
    {
        protect &= 0xFF;
        return protect == PAGE_EXECUTE || protect == PAGE_EXECUTE_READ || protect == PAGE_EXECUTE_READWRITE || protect == PAGE_EXECUTE_WRITECOPY;
    }

    std::optional<ModuleInfo> GetModuleInfo(const wchar_t *moduleName)
    {
        HMODULE mod = nullptr;

        if (moduleName == nullptr)
        {
            mod = GetModuleHandleW(nullptr);
        }
        else
        {
            mod = GetModuleHandleW(moduleName);
        }

        if (!mod)
            return std::nullopt;

        MODULEINFO mi{};
        if (!GetModuleInformation(GetCurrentProcess(), mod, &mi, sizeof(mi)))
            return std::nullopt;

        ModuleInfo out;
        out.handle = mod;
        out.base = reinterpret_cast<std::uintptr_t>(mi.lpBaseOfDll);
        out.size = static_cast<std::size_t>(mi.SizeOfImage);
        return out;
    }

    std::vector<PatternByte> ParsePattern(const std::string &pattern)
    {
        std::vector<PatternByte> out;
        std::istringstream iss(pattern);
        std::string tok;

        while (iss >> tok)
        {
            if (tok == "?" || tok == "??")
            {
                out.push_back(PatternByte{0, true});
                continue;
            }

            unsigned int v = 0;
            std::stringstream ss;
            ss << std::hex << tok;
            ss >> v;

            out.push_back(PatternByte{static_cast<std::uint8_t>(v & 0xFF), false});
        }

        return out;
    }

    static std::optional<std::uintptr_t> FindInRange(const std::uint8_t *base, std::size_t size, const std::vector<PatternByte> &pat) {
        if (pat.empty() || size < pat.size())
            return std::nullopt;

        for (std::size_t i = 0; i <= size - pat.size(); ++i)
        {
            bool ok = true;
            for (std::size_t j = 0; j < pat.size(); ++j)
            {
                if (!pat[j].wildcard && base[i + j] != pat[j].value)
                {
                    ok = false;
                    break;
                }
            }
            if (ok)
            {
                return reinterpret_cast<std::uintptr_t>(base + i);
            }
        }
        return std::nullopt;
    }

    std::optional<std::uintptr_t> FindSignature(const ModuleInfo &mod, const std::string &pattern, bool executableOnly) {
        if (!mod.base || !mod.size)
            return std::nullopt;

        const auto pat = ParsePattern(pattern);
        if (pat.empty())
            return std::nullopt;

        const std::uint8_t *start = reinterpret_cast<const std::uint8_t *>(mod.base);
        const std::uint8_t *end = start + mod.size;

        MEMORY_BASIC_INFORMATION mbi{};
        for (const std::uint8_t *p = start; p < end;)
        {
            if (!VirtualQuery(p, &mbi, sizeof(mbi)))
                break;

            const auto regionBase = reinterpret_cast<const std::uint8_t *>(mbi.BaseAddress);
            const std::size_t regionSize = static_cast<std::size_t>(mbi.RegionSize);

            const std::uint8_t *r0 = (regionBase > start) ? regionBase : start;
            const std::uint8_t *regionEnd = regionBase + regionSize;
            const std::uint8_t *r1 = (regionEnd < end) ? regionEnd : end;

            const bool committed = (mbi.State == MEM_COMMIT);
            const bool guarded = (mbi.Protect & PAGE_GUARD) != 0;
            const bool readable = IsReadable(mbi.Protect);
            const bool execOk = !executableOnly || IsExecutable(mbi.Protect);

            if (committed && !guarded && readable && execOk && r1 > r0)
            {
                if (auto hit = FindInRange(r0, static_cast<std::size_t>(r1 - r0), pat))
                {
                    return hit;
                }
            }

            p = regionBase + regionSize;
        }

        return std::nullopt;
    }

    bool Protect(void *address, std::size_t size, DWORD newProtect, DWORD *oldProtect)
    {
        return VirtualProtect(address, size, newProtect, oldProtect) != 0;
    }

    std::uintptr_t ResolveRipRelative(std::uintptr_t instrAddress, std::size_t instrLen, std::ptrdiff_t dispOffset)
    {
        const auto dispPtr = reinterpret_cast<const std::int32_t *>(instrAddress + static_cast<std::uintptr_t>(dispOffset));
        const std::int32_t disp = *dispPtr;
        return instrAddress + instrLen + static_cast<std::intptr_t>(disp);
    }

}
