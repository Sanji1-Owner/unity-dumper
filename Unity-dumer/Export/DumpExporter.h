#pragma once

#include "../Core/RuntimeContext.h"
#include "../Scripts/ScriptAnalyzer.h"
#include <filesystem>

namespace UnityAnalyzer::Export {

class DumpExporter {
public:
    void Export(const Core::RuntimeContext& context, const std::vector<Scripts::ScriptEntry>& scripts);

private:
    void CreateDirectoryStructure(const std::filesystem::path& root);
    void SaveJson(const nlohmann::json& j, const std::filesystem::path& path);
};

} // namespace UnityAnalyzer::Export
