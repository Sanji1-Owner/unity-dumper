#pragma once
#include "../Metadata/MetadataReader.h"
#include <nlohmann/json.hpp>

namespace UnityAnalyzer::IL2CPP::Analysis {

class MetadataAnalyzer {
public:
    MetadataAnalyzer(const Metadata::MetadataReader& reader);
    
    nlohmann::json Analyze();

private:
    const Metadata::MetadataReader& reader_;
};

} // namespace UnityAnalyzer::IL2CPP::Analysis
