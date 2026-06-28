#include "AssemblyScanner.h"
#include "../Core/ResourceEntry.h"
#include <iostream>

namespace UnityAnalyzer::Runtime {

AssemblyScanner::AssemblyScanner(Il2CppRuntimeBridge& bridge) : bridge_(bridge) {}

std::vector<Il2CppImageInfo> AssemblyScanner::Scan(Core::RuntimeContext& context) {
    std::vector<Il2CppImageInfo> images;
    void* domain = bridge_.GetDomain();
    if (!domain) return images;

    auto assemblies = bridge_.GetAssemblies(domain);
    auto index = context.GetResourceIndex();

    for (void* assembly : assemblies) {
        void* image = bridge_.GetImage(assembly);
        if (image) {
            Il2CppImageInfo info;
            info.handle = image;
            info.name = bridge_.GetImageName(image);
            info.assembly_name = info.name; // In IL2CPP they are often identical or very similar
            images.push_back(info);

            // Register as an Assembly resource
            Core::ResourceEntry entry;
            entry.unique_id = "runtime_assembly_" + info.name;
            entry.name = info.name;
            entry.resource_type = Core::ResourceType::Assembly;
            entry.metadata["runtime_handle"] = std::to_string((uintptr_t)image);
            
            index->AddResource(entry);
        }
    }

    return images;
}

} // namespace UnityAnalyzer::Runtime
