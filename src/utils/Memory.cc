#include "Memory.h"
#include "ProcessHelper.h"

#include <vector>
#include <sstream>
#include <Windows.h>

// No test

namespace mem
{
    static std::vector<int> PatternToBytes(const char* pattern)
    {
        std::vector<int> bytes;
        std::istringstream stream(pattern);
        std::string byte;

        while (stream >> byte)
        {
            if (byte == "?" || byte == "??")
                bytes.push_back(-1);
            else
                bytes.push_back(strtoul(byte.c_str(), nullptr, 16));
        }

        return bytes;
    }

    uintptr_t FindPattern(const char* signature)
    {
        auto base = ProcessHelper::getExecutableBase();
        auto size = ProcessHelper::getModuleSize(base);

        if (!base || !size)
            return 0;

        auto patternBytes = PatternToBytes(signature);
        auto scanBytes = reinterpret_cast<uint8_t*>(base);

        size_t patternSize = patternBytes.size();

        for (size_t i = 0; i < size - patternSize; i++)
        {
            bool found = true;

            for (size_t j = 0; j < patternSize; j++)
            {
                if (patternBytes[j] != -1 &&
                    scanBytes[i + j] != patternBytes[j])
                {
                    found = false;
                    break;
                }
            }

            if (found)
                return (uintptr_t)&scanBytes[i];
        }

        return 0;
    }
}
