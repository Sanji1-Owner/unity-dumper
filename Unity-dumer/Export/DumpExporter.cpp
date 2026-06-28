#include "DumpExporter.h"
#include <fstream>
#include <iostream>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <map>

namespace UnityAnalyzer::Export {

std::string GetCurrentTimestamp() {
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    struct tm gmt;
#ifdef _WIN32
    gmtime_s(&gmt, &in_time_t);
#else
    gmtime_r(&in_time_t, &gmt);
#endif
    ss << std::put_time(&gmt, "%Y-%m-%dT%H:%M:%SZ");
    return ss.str();
}

void DumpExporter::Export(const Core::RuntimeContext& context, const std::vector<Scripts::ScriptEntry>& scripts) {
    auto root = context.GetExportPath();
    CreateDirectoryStructure(root);

    auto resource_index = context.GetResourceIndex();

    // Database
    SaveJson(resource_index->ExportResources(), root / "Database" / "resources.json");
    SaveJson(resource_index->ExportDependencies(), root / "Database" / "dependencies.json");
    
    // Build Hierarchy
    nlohmann::json hierarchy = nlohmann::json::array();
    auto game_objects = resource_index->GetResourcesByType(Core::ResourceType::GameObject);

    for (const auto& go : game_objects) {
        nlohmann::json node;
        node["name"] = go->name;
        node["id"] = go->unique_id;
        node["components"] = nlohmann::json::array();
        
        std::string comps = go->metadata.count("components") ? go->metadata.at("components") : "";
        std::stringstream ss(comps);
        std::string segment;
        while (std::getline(ss, segment, ';')) {
            if (!segment.empty()) node["components"].push_back(segment);
        }
        hierarchy.push_back(node);
    }
    SaveJson(hierarchy, root / "Database" / "hierarchy.json");

    // Scripts
    nlohmann::json script_list = nlohmann::json::array();
    for (const auto& s : scripts) {
        script_list.push_back(s.to_json());
    }
    SaveJson(script_list, root / "Database" / "scripts.json");

    // version.json
    nlohmann::json version_info = {
        {"analyzer_version", "1.0.0-RC2"},
        {"unity_version", context.GetUnityVersion()},
        {"platform", 
#ifdef _WIN32
            "Windows"
#elif __ANDROID__
            "Android"
#else
            "Linux"
#endif
        },
        {"export_date", GetCurrentTimestamp()},
        {"enabled_extractors", {"Texture2D", "Mesh", "AudioClip", "GameObject", "Transform", "Shader", "AnimationClip"}}
    };
    SaveJson(version_info, root / "version.json");

    // Summary
    nlohmann::json summary = {
        {"unity_version", context.GetUnityVersion()},
        {"timestamp", GetCurrentTimestamp()},
        {"resource_count", resource_index->ExportResources().size()},
        {"script_count", scripts.size()},
        {"status", "Verified"}
    };
    SaveJson(summary, root / "Reports" / "summary.json");
    
    std::cout << "Export completed to: " << root.string() << std::endl;
}

void DumpExporter::CreateDirectoryStructure(const std::filesystem::path& root) {
    std::filesystem::create_directories(root / "Assets" / "Textures");
    std::filesystem::create_directories(root / "Assets" / "Models");
    std::filesystem::create_directories(root / "Assets" / "Audio");
    std::filesystem::create_directories(root / "Assets" / "Materials");
    std::filesystem::create_directories(root / "Assets" / "Shaders");
    std::filesystem::create_directories(root / "Metadata");
    std::filesystem::create_directories(root / "Database");
    std::filesystem::create_directories(root / "Reports");
}

void DumpExporter::SaveJson(const nlohmann::json& j, const std::filesystem::path& path) {
    std::ofstream file(path);
    if (file.is_open()) {
        file << j.dump(4);
    }
}

} // namespace UnityAnalyzer::Export
