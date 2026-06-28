#pragma once

#include <string>
#include <filesystem>
#include <nlohmann/json.hpp>

namespace UnityAnalyzer::Core {

struct Config {
    std::filesystem::path exportPath = "DumpSession";
    bool enableIl2Cpp = true;
    bool enableMono = true;
    bool enableAssets = true;
    int threadCount = 4;
    std::string outputFormat = "json";

    void Load(const std::filesystem::path& path) {
        std::ifstream f(path);
        if (!f.is_open()) return;
        nlohmann::json j = nlohmann::json::parse(f);
        if (j.contains("exportPath")) exportPath = j["exportPath"].get<std::string>();
        if (j.contains("enableIl2Cpp")) enableIl2Cpp = j["enableIl2Cpp"];
        if (j.contains("enableMono")) enableMono = j["enableMono"];
        if (j.contains("enableAssets")) enableAssets = j["enableAssets"];
        if (j.contains("threadCount")) threadCount = j["threadCount"];
    }

    void Save(const std::filesystem::path& path) {
        nlohmann::json j;
        j["exportPath"] = exportPath.string();
        j["enableIl2Cpp"] = enableIl2Cpp;
        j["enableMono"] = enableMono;
        j["enableAssets"] = enableAssets;
        j["threadCount"] = threadCount;
        std::ofstream f(path);
        f << j.dump(4);
    }
};

} // namespace UnityAnalyzer::Core
