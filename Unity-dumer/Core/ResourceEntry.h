#pragma once

#include <string>
#include <vector>
#include <map>
#include <nlohmann/json.hpp>

namespace UnityAnalyzer::Core {

enum class ResourceType {
    Unknown,
    Texture,
    Sprite,
    Mesh,
    Model,
    Material,
    Shader,
    Animation,
    Animator,
    AudioClip,
    Prefab,
    Scene,
    Script,
    Assembly,
    AssetBundle,
    Addressable,
    SerializedFile,
    Class,
    Field
};

struct ResourceEntry {
    std::string unique_id;
    ResourceType resource_type = ResourceType::Unknown;
    std::string name;
    std::string path;
    std::string hash;
    size_t size = 0;
    std::string parent;
    std::vector<std::string> children;
    std::vector<std::string> dependencies;
    std::map<std::string, std::string> metadata;

    nlohmann::json to_json() const {
        return {
            {"unique_id", unique_id},
            {"type", static_cast<int>(resource_type)},
            {"name", name},
            {"path", path},
            {"hash", hash},
            {"size", size},
            {"parent", parent},
            {"children", children},
            {"dependencies", dependencies},
            {"metadata", metadata}
        };
    }
};

} // namespace UnityAnalyzer::Core
