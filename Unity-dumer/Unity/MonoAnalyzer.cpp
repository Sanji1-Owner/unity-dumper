#include "MonoAnalyzer.h"
#include <filesystem>
#include <algorithm>

namespace UnityAnalyzer::Unity {

bool MonoAnalyzer::Initialize(Core::RuntimeContext& context) {
    return context.GetRuntimeInfo().backend == Core::ScriptingBackend::Mono;
}

void MonoAnalyzer::ScanAssemblies() {
    // In a real runtime environment, we would use mono_domain_get_assemblies
    // and iterate through MonoAssembly/MonoImage to get class/method info.
    
    assemblies_.push_back("UnityEngine.dll");
    assemblies_.push_back("UnityEngine.UI.dll");
    assemblies_.push_back("Assembly-CSharp.dll");
}

nlohmann::json MonoAnalyzer::ExportMetadata() const {
    nlohmann::json json = nlohmann::json::array();
    for (const auto& asm_name : assemblies_) {
        json.push_back({{"name", asm_name}});
    }
    return json;
}

} // namespace UnityAnalyzer::Unity
