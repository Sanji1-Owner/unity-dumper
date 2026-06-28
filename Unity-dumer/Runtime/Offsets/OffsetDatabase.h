#pragma once

#include "OffsetProfile.h"
#include <vector>
#include <string>

namespace UnityAnalyzer::Runtime::Offsets {

class OffsetDatabase {
public:
    void AddProfile(const OffsetProfile& profile) {
        profiles_.push_back(profile);
    }

    const OffsetProfile* FindProfile(const std::string& unity_version, const std::string& platform, Architecture arch) const {
        for (const auto& p : profiles_) {
            if (p.unity_version == unity_version && p.platform == platform && p.arch == arch) {
                return &p;
            }
        }
        return nullptr;
    }

    const std::vector<OffsetProfile>& GetProfiles() const { return profiles_; }

private:
    std::vector<OffsetProfile> profiles_;
};

} // namespace UnityAnalyzer::Runtime::Offsets
