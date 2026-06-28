#include "OffsetManager.h"
#include "OffsetLoader.h"
#include "../../Core/Logger.h"
#include <algorithm>

namespace UnityAnalyzer::Runtime::Offsets {

OffsetManager& OffsetManager::Instance() {
    static OffsetManager instance;
    return instance;
}

bool OffsetManager::Initialize(const std::string& profiles_dir) {
    std::lock_guard<std::mutex> lock(mutex_);
    Core::Logger::Instance().Info("Initializing OffsetManager with directory: " + profiles_dir);
    return OffsetLoader::LoadFromDirectory(profiles_dir, database_);
}

bool OffsetManager::SelectProfile(const std::string& unity_version, const std::string& platform, Architecture arch) {
    std::lock_guard<std::mutex> lock(mutex_);
    
    // Exact match
    current_profile_ = database_.FindProfile(unity_version, platform, arch);
    
    if (!current_profile_) {
        // Partial match (e.g., major version)
        for (const auto& p : database_.GetProfiles()) {
            if (unity_version.find(p.unity_version) == 0 && p.platform == platform && p.arch == arch) {
                current_profile_ = &p;
                break;
            }
        }
    }

    if (current_profile_) {
        Core::Logger::Instance().Info("Selected offset profile: " + current_profile_->name);
        return true;
    }

    Core::Logger::Instance().Error("Failed to find suitable offset profile for Unity " + unity_version + " on " + platform);
    return false;
}

bool OffsetManager::LoadManualProfile(const std::string& path) {
    std::lock_guard<std::mutex> lock(mutex_);
    OffsetProfile profile;
    if (OffsetLoader::LoadFromFile(path, profile)) {
        // We add it to DB first to have a persistent copy (if DB stores by value)
        database_.AddProfile(profile);
        // Find it back to get the pointer to the stored version
        current_profile_ = database_.FindProfile(profile.unity_version, profile.platform, profile.arch);
        return true;
    }
    return false;
}

uintptr_t OffsetManager::GetOffset(const std::string& name) const {
    std::lock_guard<std::mutex> lock(mutex_);
    if (!current_profile_) return 0;
    return current_profile_->Get(name);
}

} // namespace UnityAnalyzer::Runtime::Offsets
