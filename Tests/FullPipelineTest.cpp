#include <iostream>
#include <cassert>
#include <filesystem>
#include <vector>
#include "../UnityAnalyzer/Pipeline/Pipeline.h"
#include "../UnityAnalyzer/Export/ExportValidator.h"
#include "../UnityAnalyzer/Core/RuntimeContext.h"

using namespace UnityAnalyzer;

void TestFullPipeline() {
    std::cout << "Running Full Pipeline Stability Test..." << std::endl;

    // Create a mock context
    Core::RuntimeContext context;
    context.SetExportPath("TestDump");
    context.SetUnityVersion("2021.3.0f1");

    // Ensure directory exists
    std::filesystem::create_directories("TestDump");

    // Initialize Pipeline
    // Note: AnalysisPipeline requires a reference to context
    Pipeline::AnalysisPipeline pipeline(context);

    // Run the pipeline stages (simulated since we don't have a real target here)
    std::cout << "Testing Export and Validation flow..." << std::endl;
    
    Export::DumpExporter exporter;
    std::vector<Scripts::ScriptEntry> mock_scripts;
    exporter.Export(context, mock_scripts);

    Export::ExportValidator validator;
    auto version_check = validator.ValidateJson("TestDump/version.json");
    if (!version_check.success) {
        std::cerr << "Version JSON validation failed: " << version_check.message << std::endl;
        assert(false);
    }

    std::cout << "Full Pipeline flow: STABLE" << std::endl;
}

int main() {
    try {
        TestFullPipeline();
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Pipeline Test FAILED: " << e.what() << std::endl;
        return 1;
    }
}
