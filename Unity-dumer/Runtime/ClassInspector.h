#pragma once

#include "Il2CppRuntimeBridge.h"
#include "../Core/RuntimeContext.h"
#include <string>
#include <vector>

namespace UnityAnalyzer::Runtime {

struct Il2CppClassInfo {
    void* handle;
    std::string name;
    std::string namespaze;
    std::string assembly_id;
};

class ClassInspector {
public:
    explicit ClassInspector(Il2CppRuntimeBridge& bridge);

    void InspectImage(Core::RuntimeContext& context, void* image_handle, const std::string& assembly_id);

private:
    Il2CppRuntimeBridge& bridge_;
};

} // namespace UnityAnalyzer::Runtime
