#include "Pipeline.h"
#include "../Unity/UnityRuntimeDetector.h"
#include "../Archives/UnityFSParser.h"
#include "../Scripts/ScriptAnalyzer.h"
#include "../Export/DumpExporter.h"
#include "../Core/EventSystem.h"
#include <iostream>
#include <filesystem>

namespace UnityAnalyzer::Pipeline {

AnalysisPipeline::AnalysisPipeline(Core::RuntimeContext& context) : context_(context) {}

void AnalysisPipeline::Run() {
    auto& events = Core::EventSystem::Instance();
    events.Publish(Core::EventType::DetectionStarted);
    
    std::cout << "Starting Analysis Pipeline..." << std::endl;
    
    // Load external plugins
    module_loader_.LoadPlugins(context_.GetExportPath().parent_path() / "plugins");
    module_loader_.InitializeAll(context_);

    StageDetect();
    StageIndex();
    StageAnalyze();
    
    module_loader_.ScanAll();

    StageExport();

    events.Publish(Core::EventType::ExportFinished);
    std::cout << "Pipeline Execution Finished." << std::endl;
}

void AnalysisPipeline::StageDetect() {
    std::cout << "[Stage 1: Detect] Detecting Unity Runtime..." << std::endl;
    Unity::UnityRuntimeDetector detector;
    if (detector.Detect(context_)) {
        std::cout << "Unity version detected: " << context_.GetUnityVersion() << std::endl;
    } else {
        std::cout << "Unity runtime not detected. Proceeding with generic file analysis." << std::endl;
    }
}

void AnalysisPipeline::StageIndex() {
    std::cout << "[Stage 2: Index] Indexing resources from archives..." << std::endl;
    
    // Scan for Unity bundles in the target directory (determined by context or current path)
    std::filesystem::path search_path = context_.GetExportPath().parent_path();
    Archives::UnityFSParser parser;
    
    bool found = false;
    if (std::filesystem::exists(search_path)) {
        for (const auto& entry : std::filesystem::recursive_directory_iterator(search_path)) {
            if (entry.is_regular_file()) {
                std::string ext = entry.path().extension().string();
                if (ext == ".unity3d" || ext == ".bundle" || ext == ".assets") {
                    std::cout << " Found archive: " << entry.path().filename() << std::endl;
                    if (parser.Parse(entry.path(), context_)) {
                        found = true;
                    }
                }
            }
        }
    }

    if (!found) {
        std::cout << " No Unity archives found in " << search_path << ". Resource index may be empty." << std::endl;
    }
}

void AnalysisPipeline::StageAnalyze() {
    std::cout << "[Stage 3: Analyze] Analyzing scripts and assemblies..." << std::endl;
    Scripts::AssemblyAnalyzer asmAnalyzer;
    auto assemblies = asmAnalyzer.Analyze();
    discovered_scripts_.insert(discovered_scripts_.end(), assemblies.begin(), assemblies.end());

    Scripts::LuaAnalyzer luaAnalyzer;
    auto luaScripts = luaAnalyzer.Analyze();
    discovered_scripts_.insert(discovered_scripts_.end(), luaScripts.begin(), luaScripts.end());
    
    std::cout << "Found " << discovered_scripts_.size() << " script/assembly entries." << std::endl;
}

void AnalysisPipeline::StageExport() {
    std::cout << "[Stage 4: Export] Exporting analysis dump..." << std::endl;
    Export::DumpExporter exporter;
    exporter.Export(context_, discovered_scripts_);
}

} // namespace UnityAnalyzer::Pipeline
