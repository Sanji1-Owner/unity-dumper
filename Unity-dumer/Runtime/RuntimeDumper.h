#pragma once

#include "Il2CppRuntimeBridge.h"
#include "AssemblyScanner.h"
#include "ClassInspector.h"
#include "../Core/RuntimeContext.h"
#include <memory>

namespace UnityAnalyzer::Runtime {

class RuntimeDumper {
public:
    explicit RuntimeDumper(Core::RuntimeContext& context);
    
    bool Connect();
    void Dump();

private:
    Core::RuntimeContext& context_;
    std::unique_ptr<Il2CppRuntimeBridge> bridge_;
    std::unique_ptr<AssemblyScanner> scanner_;
    std::unique_ptr<ClassInspector> inspector_;
};

} // namespace UnityAnalyzer::Runtime
