#pragma once

#include "OffsetProfile.h"
#include "OffsetDatabase.h"
#include <filesystem>
#include <fstream>
#include <nlohmann/json.hpp>

namespace UnityAnalyzer::Runtime::Offsets {

class OffsetLoader {
public:
    static bool LoadFromDirectory(const std::filesystem::path& directory, OffsetDatabase& db) {
        if (!std::filesystem::exists(directory) || !std::filesystem::is_directory(directory)) {
            return false;
        }

        for (const auto& entry : std::filesystem::directory_iterator(directory)) {
            if (entry.path().extension() == ".json") {
                OffsetProfile profile;
                if (LoadFromFile(entry.path(), profile)) {
                    db.AddProfile(profile);
                }
            }
        }
        return true;
    }

    static bool LoadFromFile(const std::filesystem::path& path, OffsetProfile& profile) {
        std::ifstream file(path);
        if (!file.is_open()) return false;

        try {
            nlohmann::json j;
            file >> j;

            profile.name = j.value("name", "");
            profile.unity_version = j.value("unity_version", "");
            profile.platform = j.value("platform", "");
            
            std::string arch_str = j.value("architecture", "Unknown");
            if (arch_str == "x86") profile.arch = Architecture::x86;
            else if (arch_str == "x64") profile.arch = Architecture::x64;
            else if (arch_str == "ARM") profile.arch = Architecture::ARM;
            else if (arch_str == "ARM64") profile.arch = Architecture::ARM64;
            else profile.arch = Architecture::Unknown;

            if (j.contains("offsets")) {
                for (auto& el : j["offsets"].items()) {
                    profile.offsets[el.key()] = el.value().get<uintptr_t>();
                }
            }
            return true;
        } catch (...) {
            return false;
        }
    }
};

} // namespace UnityAnalyzer::Runtime::Offsets
