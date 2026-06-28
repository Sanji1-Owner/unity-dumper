#pragma once

#include <string>
#include <map>
#include <memory>

namespace UnityAnalyzer::Unity {

struct UnityOffsets {
    uintptr_t GameObject_Name = 0x0;
    uintptr_t Object_Class = 0x0;
    // ... add more as needed for different versions
};

class IVersionAdapter {
public:
    virtual ~IVersionAdapter() = default;
    virtual std::string GetVersionRange() const = 0;
    virtual UnityOffsets GetOffsets() const = 0;
};

class Unity2022Adapter : public IVersionAdapter {
public:
    std::string GetVersionRange() const override { return "2022.*"; }
    UnityOffsets GetOffsets() const override {
        UnityOffsets offsets;
        offsets.GameObject_Name = 0x30;
        return offsets;
    }
};

class VersionAdapterFactory {
public:
    static std::unique_ptr<IVersionAdapter> CreateAdapter(const std::string& version) {
        if (version.find("2022") != std::string::npos) {
            return std::make_unique<Unity2022Adapter>();
        }
        // Add more mappings here
        return nullptr;
    }
};

} // namespace UnityAnalyzer::Unity
