#pragma once

#include "OffsetProfile.h"
#include "OffsetDatabase.h"
#include <string>
#include <memory>
#include <mutex>

namespace UnityAnalyzer::Runtime::Offsets {

class OffsetManager {
public:
    static OffsetManager& Instance();

    bool Initialize(const std::string& profiles_dir);
    
    // Select profile based on detected environment
    bool SelectProfile(const std::string& unity_version, const std::string& platform, Architecture arch);
    
    // Manually load a specific profile file
    bool LoadManualProfile(const std::string& path);

    // Get an offset by name. Returns 0 if not found or no profile selected.
    uintptr_t GetOffset(const std::string& name) const;

    const OffsetProfile* GetCurrentProfile() const { return current_profile_; }

private:
    OffsetManager() = default;
    
    OffsetDatabase database_;
    const OffsetProfile* current_profile_ = nullptr;
    mutable std::mutex mutex_;
};

} // namespace UnityAnalyzer::Runtime::Offsets
