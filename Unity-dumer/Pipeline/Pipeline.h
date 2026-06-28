#pragma once

#include "../Core/RuntimeContext.h"
#include "../Unity/UnityRuntimeDetector.h"
#include "../Scripts/ScriptAnalyzer.h"
#include "../Export/DumpExporter.h"
#include "../Plugins/ModuleLoader.h"

namespace UnityAnalyzer::Pipeline {

class AnalysisPipeline {
public:
    AnalysisPipeline(Core::RuntimeContext& context);

    void Run();

private:
    void StageDetect();
    void StageIndex();
    void StageAnalyze();
    void StageExport();

    Core::RuntimeContext& context_;
    Plugins::ModuleLoader module_loader_;
    std::vector<Scripts::ScriptEntry> discovered_scripts_;
};

} // namespace UnityAnalyzer::Pipeline
