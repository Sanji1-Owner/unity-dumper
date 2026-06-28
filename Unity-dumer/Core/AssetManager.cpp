#include "AssetManager.h"
#include <filesystem>
#include <iostream>

namespace UnityAnalyzer::Core {

void AssetManager::IndexAssets(RuntimeContext& context) {
    DiscoverSerializedFiles(context);
    
    // In a real implementation, this would iterate through all loaded Unity assets
    // using internal engine iterators (Resources.FindObjectsOfTypeAll equivalent)
}

void AssetManager::DiscoverSerializedFiles(RuntimeContext& context) {
    // Search for .assets, .sharedassets, level* files in the data folder
    // For now we simulate finding common Unity assets
    ProcessResource(context, "Resources/unity_default_resources", ResourceType::SerializedFile);
}

void AssetManager::ProcessResource(RuntimeContext& context, const std::string& path, ResourceType type) {
    auto index = context.GetResourceIndex();
    
    ResourceEntry entry;
    entry.name = std::filesystem::path(path).filename().string();
    entry.path = path;
    entry.resource_type = type;
    entry.unique_id = "asset_" + entry.name;
    
    index->AddResource(entry);
}

} // namespace UnityAnalyzer::Core
