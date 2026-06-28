#pragma once

#include "Il2CppRuntimeBridge.h"
#include "../Core/RuntimeContext.h"
#include <vector>
#include <string>

namespace UnityAnalyzer::Runtime {

struct Il2CppImageInfo {
    void* handle;
    std::string name;
    std::string assembly_name;
};

class AssemblyScanner {
public:
    explicit AssemblyScanner(Il2CppRuntimeBridge& bridge);
    
    std::vector<Il2CppImageInfo> Scan(Core::RuntimeContext& context);

private:
    Il2CppRuntimeBridge& bridge_;
};

} // namespace UnityAnalyzer::Runtime
