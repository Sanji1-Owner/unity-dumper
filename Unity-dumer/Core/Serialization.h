#pragma once

#include <nlohmann/json.hpp>
#include <fstream>
#include <filesystem>

namespace UnityAnalyzer::Core {

class Serialization {
public:
    static bool SaveToJson(const nlohmann::json& j, const std::filesystem::path& path) {
        std::ofstream file(path);
        if (file.is_open()) {
            file << j.dump(4);
            return true;
        }
        return false;
    }

    static nlohmann::json LoadFromJson(const std::filesystem::path& path) {
        if (!std::filesystem::exists(path)) return {};
        std::ifstream file(path);
        nlohmann::json j;
        if (file.is_open()) {
            file >> j;
        }
        return j;
    }
};

} // namespace UnityAnalyzer::Core
