#pragma once
#include <string>
#include <filesystem>
#include "../Metadata/MetadataReader.h"

namespace UnityAnalyzer::IL2CPP::Export {

class SDKExporter {
public:
    SDKExporter(const Metadata::MetadataReader& reader);
    
    bool Export(const std::filesystem::path& outputDir);

private:
    const Metadata::MetadataReader& reader_;
    
    void ExportAssembly(const std::filesystem::path& assemblyDir, int32_t imageIndex);
};

} // namespace UnityAnalyzer::IL2CPP::Export
