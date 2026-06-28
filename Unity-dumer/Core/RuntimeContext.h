#pragma once

#include "ResourceIndex.h"
#include <string>
#include <vector>
#include <memory>
#include <filesystem>

namespace UnityAnalyzer::Core {

enum class ScriptingBackend {
    Unknown,
    Mono,
    IL2CPP
};

struct ModuleInfo {
    std::string name;
    std::string path;
    uintptr_t base_address;
    size_t size;
};

struct RuntimeInformation {
    std::string unity_version;
    ScriptingBackend backend = ScriptingBackend::Unknown;
    std::string platform;
    uint32_t process_id = 0;
    std::vector<ModuleInfo> loaded_modules;
};

class RuntimeContext {
public:
    RuntimeContext() : resource_index_(std::make_shared<ResourceIndex>()) {}

    std::shared_ptr<ResourceIndex> GetResourceIndex() const { return resource_index_; }
    
    RuntimeInformation& GetRuntimeInfo() { return runtime_info_; }
    const RuntimeInformation& GetRuntimeInfo() const { return runtime_info_; }

    void SetExportPath(const std::filesystem::path& path) { export_path_ = path; }
    std::filesystem::path GetExportPath() const { return export_path_; }

private:
    std::shared_ptr<ResourceIndex> resource_index_;
    RuntimeInformation runtime_info_;
    std::filesystem::path export_path_;
};

} // namespace UnityAnalyzer::Core
