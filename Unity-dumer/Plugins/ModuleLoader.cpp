#include "ModuleLoader.h"
#include <iostream>

#ifdef _WIN32
#include <windows.h>
#else
#include <dlfcn.h>
#endif

namespace UnityAnalyzer::Plugins {

void ModuleLoader::LoadPlugins(const std::filesystem::path& pluginDir) {
    if (!std::filesystem::exists(pluginDir)) return;

    for (const auto& entry : std::filesystem::directory_iterator(pluginDir)) {
        auto ext = entry.path().extension();
        if (ext == ".dll" || ext == ".so") {
            void* handle = LoadLibraryInternal(entry.path());
            if (handle) {
                auto createFunc = (IUnityModule*(*)())GetProcAddressInternal(handle, "createUnityModule");
                if (createFunc) {
                    auto module = std::shared_ptr<IUnityModule>(createFunc());
                    if (module && module->abiVersion() == 1) {
                        modules_.push_back(module);
                        handles_.push_back(handle);
                        std::cout << "Loaded plugin: " << module->name() << std::endl;
                    }
                }
            }
        }
    }
}

void* ModuleLoader::LoadLibraryInternal(const std::filesystem::path& path) {
#ifdef _WIN32
    return (void*)LoadLibraryW(path.c_str());
#else
    return dlopen(path.c_str(), RTLD_LAZY);
#endif
}

void* ModuleLoader::GetProcAddressInternal(void* handle, const std::string& name) {
#ifdef _WIN32
    return (void*)GetProcAddress((HMODULE)handle, name.c_str());
#else
    return dlsym(handle, name.c_str());
#endif
}

} // namespace UnityAnalyzer::Plugins
