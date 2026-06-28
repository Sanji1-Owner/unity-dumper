#pragma once

#include "../Core/RuntimeContext.h"
#include <nlohmann/json.hpp>
#include <string>
#include <vector>

namespace UnityAnalyzer::Unity {

class MonoAnalyzer {
public:
    bool Initialize(Core::RuntimeContext& context);
    void ScanAssemblies();
    nlohmann::json ExportMetadata() const;

private:
    std::vector<std::string> assemblies_;
};

} // namespace UnityAnalyzer::Unity
