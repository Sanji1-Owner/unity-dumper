#pragma once

#include <string>
#include <vector>
#include <map>
#include <nlohmann/json.hpp>

namespace UnityAnalyzer::Scripts {

struct ScriptEntry {
    std::string id;
    std::string name;
    std::string path;
    std::string language; // "C#", "Lua"
    std::vector<std::string> references;
    std::map<std::string, std::string> metadata;

    nlohmann::json to_json() const {
        return {
            {"id", id},
            {"name", name},
            {"path", path},
            {"language", language},
            {"references", references},
            {"metadata", metadata}
        };
    }
};

} // namespace UnityAnalyzer::Scripts
