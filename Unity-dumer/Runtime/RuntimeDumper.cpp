#include "RuntimeDumper.h"
#include <iostream>

namespace UnityAnalyzer::Runtime {

RuntimeDumper::RuntimeDumper(Core::RuntimeContext& context) 
    : context_(context) {}

bool RuntimeDumper::Connect() {
    bridge_ = std::make_unique<Il2CppRuntimeBridge>();
    if (!bridge_->Initialize()) {
        return false;
    }

    scanner_ = std::make_unique<AssemblyScanner>(*bridge_);
    inspector_ = std::make_unique<ClassInspector>(*bridge_);
    return true;
}

void RuntimeDumper::Dump() {
    std::cout << "[RuntimeDumper] Starting IL2CPP Runtime Dump..." << std::endl;
    
    auto images = scanner_->Scan(context_);
    std::cout << "[RuntimeDumper] Found " << images.size() << " assemblies." << std::endl;

    for (const auto& image : images) {
        std::string assembly_id = "runtime_assembly_" + image.name;
        inspector_->InspectImage(context_, image.handle, assembly_id);
    }

    std::cout << "[RuntimeDumper] Dump complete." << std::endl;
}

} // namespace UnityAnalyzer::Runtime
