#pragma once

#include "Il2CppRuntimeBridge.h"
#include <string>
#include <map>

namespace UnityAnalyzer::Runtime {

struct FieldMetadata {
    std::string name;
    size_t offset;
    void* handle;
};

class FieldInspector {
public:
    explicit FieldInspector(Il2CppRuntimeBridge& bridge) : bridge_(bridge) {}

    FieldMetadata GetFieldInfo(void* field_handle) {
        FieldMetadata meta;
        meta.handle = field_handle;
        meta.name = bridge_.GetFieldName(field_handle);
        meta.offset = bridge_.GetFieldOffset(field_handle);
        return meta;
    }

private:
    Il2CppRuntimeBridge& bridge_;
};

} // namespace UnityAnalyzer::Runtime
