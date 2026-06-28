#pragma once

#include <string>
#include <vector>
#include <filesystem>
#include "../Core/RuntimeContext.h"

namespace UnityAnalyzer::Unity {

class UnityRuntimeDetector {
public:
    bool Detect(Core::RuntimeContext& context);
    
private:
    std::string FindUnityVersion(const std::vector<Core::ModuleInfo>& modules);
    std::vector<Core::ModuleInfo> GetLoadedModules();
    Core::ScriptingBackend IdentifyBackend(const std::vector<Core::ModuleInfo>& modules);
    std::string GetPlatformName();
};

} // namespace UnityAnalyzer::Unity
