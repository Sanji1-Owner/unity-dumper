#include "ObjectInspector.h"
#include "../Core/ResourceEntry.h"
#include "../Runtime/RuntimeDumper.h" // For runtime context if needed

namespace UnityAnalyzer::Unity {

void ObjectInspector::Inspect(Core::RuntimeContext& context) {
    auto objects = ScanRuntimeObjects(context);
    auto index = context.GetResourceIndex();

    for (const auto& obj : objects) {
        Core::ResourceEntry entry;
        entry.unique_id = "unity_instance_" + std::to_string(obj.instance_id);
        entry.name = obj.name;
        entry.resource_type = Core::ResourceType::Unknown;
        entry.parent = obj.parent_id != 0 ? "unity_instance_" + std::to_string(obj.parent_id) : "";
        
        for (auto cid : obj.children_ids) {
            entry.children.push_back("unity_instance_" + std::to_string(cid));
        }

        entry.metadata = obj.metadata;
        entry.metadata["instance_id"] = std::to_string(obj.instance_id);
        entry.metadata["unity_type"] = obj.type;

        index->AddResource(entry);
    }
}

std::vector<UnityObjectEntry> ObjectInspector::ScanRuntimeObjects(Core::RuntimeContext& /*context*/) {
    // In a real integrated environment, this would use Il2CppRuntimeBridge 
    // to find UnityEngine.Object types and iterate through the active scene.
    // This part remains a stub for actual memory traversal logic.
    return {}; 
}

} // namespace UnityAnalyzer::Unity
