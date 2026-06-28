#include "UnityRuntimeDetector.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <filesystem>

#ifdef _WIN32
#include <windows.h>
#include <psapi.h>
#elif defined(__linux__) || defined(__ANDROID__)
#include <link.h>
#include <unistd.h>
#endif

namespace UnityAnalyzer::Unity {

bool UnityRuntimeDetector::Detect(Core::RuntimeContext& context) {
    auto& info = context.GetRuntimeInfo();
    info.loaded_modules = GetLoadedModules();
    
    if (info.loaded_modules.empty()) {
        return false;
    }

    info.unity_version = FindUnityVersion(info.loaded_modules);
    info.backend = IdentifyBackend(info.loaded_modules);
    info.platform = GetPlatformName();
    
#ifdef _WIN32
    info.process_id = GetCurrentProcessId();
#else
    info.process_id = getpid();
#endif

    return !info.unity_version.empty();
}

std::vector<Core::ModuleInfo> UnityRuntimeDetector::GetLoadedModules() {
    std::vector<Core::ModuleInfo> modules;
#ifdef _WIN32
    HMODULE hMods[1024];
    HANDLE hProcess = GetCurrentProcess();
    DWORD cbNeeded;

    if (EnumProcessModules(hProcess, hMods, sizeof(hMods), &cbNeeded)) {
        for (unsigned int i = 0; i < (cbNeeded / sizeof(HMODULE)); i++) {
            char szModName[MAX_PATH];
            if (GetModuleFileNameExA(hProcess, hMods[i], szModName, sizeof(szModName))) {
                MODULEINFO mi;
                if (GetModuleInformation(hProcess, hMods[i], &mi, sizeof(mi))) {
                    modules.push_back({
                        std::filesystem::path(szModName).filename().string(),
                        szModName,
                        (uintptr_t)mi.lpBaseOfDll,
                        (size_t)mi.SizeOfImage
                    });
                }
            }
        }
    }
#elif defined(__linux__) || defined(__ANDROID__)
    // Implementation using /proc/self/maps would go here for Linux/Android
#endif
    return modules;
}

std::string UnityRuntimeDetector::FindUnityVersion(const std::vector<Core::ModuleInfo>& modules) {
    for (const auto& mod : modules) {
        std::string name = mod.name;
        std::transform(name.begin(), name.end(), name.begin(), ::tolower);
        if (name.find("unityplayer") != std::string::npos || name.find("libunity") != std::string::npos) {
            // In a real implementation, we would scan the memory of this module for the version string.
            // Search pattern: "20[0-9]{2}\\.[0-9]\\.[0-9]{1,2}[abf][0-9]"
            return "2022.3.10f1"; 
        }
    }
    return "";
}

Core::ScriptingBackend UnityRuntimeDetector::IdentifyBackend(const std::vector<Core::ModuleInfo>& modules) {
    for (const auto& mod : modules) {
        std::string name = mod.name;
        std::transform(name.begin(), name.end(), name.begin(), ::tolower);
        if (name.find("il2cpp") != std::string::npos || name.find("gameassembly") != std::string::npos) {
            return Core::ScriptingBackend::IL2CPP;
        }
        if (name.find("mono") != std::string::npos || name.find("mono-2.0") != std::string::npos) {
            return Core::ScriptingBackend::Mono;
        }
    }
    return Core::ScriptingBackend::Unknown;
}

std::string UnityRuntimeDetector::GetPlatformName() {
#ifdef _WIN32
    return "Windows";
#elif defined(__ANDROID__)
    return "Android";
#elif defined(__linux__)
    return "Linux";
#else
    return "Unknown";
#endif
}

} // namespace UnityAnalyzer::Unity
