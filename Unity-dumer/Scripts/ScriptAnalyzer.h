#pragma once

#include "../Core/RuntimeContext.h"
#include "ScriptEntry.h"
#include <vector>

namespace UnityAnalyzer::Scripts {

class ScriptAnalyzer {
public:
    void Analyze(Core::RuntimeContext& context);
    const std::vector<ScriptEntry>& GetScripts() const { return scripts_; }

private:
    void ScanAssemblies(Core::RuntimeContext& context);
    void ScanLua(Core::RuntimeContext& context);
    
    std::vector<ScriptEntry> scripts_;
};

} // namespace UnityAnalyzer::Scripts
