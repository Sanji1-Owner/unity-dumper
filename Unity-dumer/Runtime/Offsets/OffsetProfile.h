#pragma once

#include <string>
#include <map>
#include <vector>

namespace UnityAnalyzer::Runtime::Offsets {

enum class Architecture {
    Unknown,
    x86,
    x64,
    ARM,
    ARM64
};

struct OffsetProfile {
    std::string name;
    std::string unity_version;
    std::string platform; // "Windows", "Android", "Linux"
    Architecture arch = Architecture::Unknown;
    
    std::map<std::string, uintptr_t> offsets;

    uintptr_t Get(const std::string& key) const {
        auto it = offsets.find(key);
        if (it != offsets.end()) return it->second;
        return 0;
    }
};

} // namespace UnityAnalyzer::Runtime::Offsets
