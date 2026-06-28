#pragma once

#include "RuntimeContext.h"
#include "ResourceEntry.h"

namespace UnityAnalyzer::Core {

class AssetManager {
public:
    void IndexAssets(RuntimeContext& context);
    
private:
    void DiscoverSerializedFiles(RuntimeContext& context);
    void ProcessResource(RuntimeContext& context, const std::string& path, ResourceType type);
};

} // namespace UnityAnalyzer::Core
