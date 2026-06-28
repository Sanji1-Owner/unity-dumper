#pragma once

#include <string>
#include <vector>
#include <memory>
#include <filesystem>
#include "IUnityModule.h"

namespace UnityAnalyzer::Plugins {

class ModuleLoader {
public:
    static ModuleLoader& Instance() {
        static ModuleLoader instance;
        return instance;
    }

    void LoadPlugins(const std::filesystem::path& pluginDir);
    const std::vector<std::shared_ptr<IUnityModule>>& GetModules() const { return modules_; }

private:
    ModuleLoader() = default;
    void* LoadLibraryInternal(const std::filesystem::path& path);
    void* GetProcAddressInternal(void* handle, const std::string& name);

    std::vector<void*> handles_;
    std::vector<std::shared_ptr<IUnityModule>> modules_;
};

} // namespace UnityAnalyzer::Plugins
