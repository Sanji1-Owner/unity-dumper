#pragma once

#include "../Core/RuntimeContext.h"
#include <nlohmann/json.hpp>
#include <string>
#include <vector>
#include <map>

namespace UnityAnalyzer::Unity {

struct Il2CppTypeInfo {
    std::string name;
    std::string namespace_name;
    std::vector<std::string> methods;
};

struct Il2CppAssemblyInfo {
    std::string name;
    std::vector<Il2CppTypeInfo> types;
};

class Il2CppAnalyzer {
public:
    bool Initialize(Core::RuntimeContext& context);
    void ScanMetadata();
    nlohmann::json ExportMetadata() const;

private:
    std::vector<Il2CppAssemblyInfo> assemblies_;
    std::filesystem::path metadata_path_;
};

} // namespace UnityAnalyzer::Unity
