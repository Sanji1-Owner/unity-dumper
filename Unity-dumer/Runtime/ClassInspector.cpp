#include "ClassInspector.h"
#include "../Core/ResourceEntry.h"
#include <iostream>

namespace UnityAnalyzer::Runtime {

ClassInspector::ClassInspector(Il2CppRuntimeBridge& bridge) : bridge_(bridge) {}

void ClassInspector::InspectImage(Core::RuntimeContext& context, void* image_handle, const std::string& assembly_id) {
    size_t class_count = bridge_.GetClassCount(image_handle);
    auto index = context.GetResourceIndex();

    for (size_t i = 0; i < class_count; ++i) {
        void* klass = bridge_.GetClass(image_handle, i);
        if (!klass) continue;

        std::string name = bridge_.GetClassName(klass);
        std::string namespaze = bridge_.GetClassNamespace(klass);
        std::string full_name = namespaze.empty() ? name : namespaze + "." + name;

        std::string class_id = assembly_id + "::" + full_name;

        Core::ResourceEntry class_entry;
        class_entry.unique_id = class_id;
        class_entry.name = name;
        class_entry.resource_type = Core::ResourceType::Class;
        class_entry.parent = assembly_id;
        class_entry.metadata["namespace"] = namespaze;
        class_entry.metadata["runtime_handle"] = std::to_string((uintptr_t)klass);

        // Inspect Fields
        void* iter = nullptr;
        while (void* field = bridge_.GetFields(klass, &iter)) {
            std::string field_name = bridge_.GetFieldName(field);
            size_t offset = bridge_.GetFieldOffset(field);

            std::string field_id = class_id + "." + field_name;

            Core::ResourceEntry field_entry;
            field_entry.unique_id = field_id;
            field_entry.name = field_name;
            field_entry.resource_type = Core::ResourceType::Field;
            field_entry.parent = class_id;
            field_entry.metadata["offset"] = std::to_string(offset);
            field_entry.metadata["runtime_handle"] = std::to_string((uintptr_t)field);

            class_entry.children.push_back(field_id);
            index->AddResource(field_entry);
        }

        index->AddResource(class_entry);
    }
}

} // namespace UnityAnalyzer::Runtime
