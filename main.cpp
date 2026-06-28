#include "UnityAnalyzer/Core/RuntimeContext.h"
#include "UnityAnalyzer/Pipeline/Pipeline.h"
#include "UnityAnalyzer/Archives/ArchiveReader.h"
#include "UnityAnalyzer/Archives/AssetBundleReader.h"
#include "UnityAnalyzer/Archives/AddressableReader.h"
#include <iostream>
#include <filesystem>

using namespace UnityAnalyzer;

int main(int argc, char* argv[]) {
    std::cout << "===============================================" << std::endl;
    std::cout << "   Universal Unity Runtime Analysis Framework  " << std::endl;
    std::cout << "===============================================" << std::endl;

    std::filesystem::path exportPath = "AnalysisDump";
    if (argc > 1) {
        exportPath = argv[1];
    }

    // 1. Initialize Registry
    auto& archiveRegistry = Archives::ArchiveRegistry::Instance();
    archiveRegistry.RegisterReader(std::make_unique<Archives::AssetBundleReader>());
    archiveRegistry.RegisterReader(std::make_unique<Archives::AddressableReader>());

    // 2. Setup Context
    Core::RuntimeContext context;
    context.SetExportPath(std::filesystem::current_path() / exportPath);

    // 3. Execute Pipeline
    Pipeline::AnalysisPipeline pipeline(context);
    
    try {
        pipeline.Run();
    } catch (const std::exception& e) {
        std::cerr << "Pipeline error: " << e.what() << std::endl;
        return 1;
    }

    std::cout << "Analysis finished successfully." << std::endl;
    return 0;
}
