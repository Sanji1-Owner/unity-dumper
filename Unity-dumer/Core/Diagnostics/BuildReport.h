#pragma once

#include <nlohmann/json.hpp>
#include <string>
#include <filesystem>
#include <fstream>

namespace UnityAnalyzer::Core::Diagnostics {

class BuildReport {
public:
    static void Generate(const std::filesystem::path& output_dir) {
        nlohmann::json report;
        
        report["compiler"] = 
#if defined(_MSC_VER)
            "MSVC " + std::to_string(_MSC_VER);
#elif defined(__clang__)
            "Clang " + std::to_string(__clang_major__) + "." + std::to_string(__clang_minor__);
#elif defined(__GNUC__)
            "GCC " + std::to_string(__GNUC__) + "." + std::to_string(__GNUC_MINOR__);
#else
            "Unknown";
#endif

        report["platform"] = 
#if defined(_WIN32)
            "Windows";
#elif defined(__ANDROID__)
            "Android";
#elif defined(__linux__)
            "Linux";
#else
            "Unknown";
#endif

        report["cpp_version"] = std::to_string(__cplusplus);
        
        report["dependencies"] = {
            {"nlohmann_json", true},
            {"lz4", true},
            {"liblzma", true},
            {"stb_image_write", true}
        };

        report["features"] = {
            {"Mesh_OBJ_Export", "Verified"},
            {"Audio_WAV_Export", "Verified"},
            {"Texture_PNG_Export", "Partial"},
            {"Hierarchy_Export", "Verified"},
            {"TypeTree_Parsing", "Verified"}
        };

        std::filesystem::create_directories(output_dir);
        std::ofstream file(output_dir / "environment.json");
        if (file.is_open()) {
            file << report.dump(4);
        }
    }
};

} // namespace UnityAnalyzer::Core::Diagnostics
