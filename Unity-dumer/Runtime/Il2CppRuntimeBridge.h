#pragma once

#include <string>
#include <vector>
#include <map>
#include <memory>

namespace UnityAnalyzer::Runtime {

class Il2CppRuntimeBridge {
public:
    Il2CppRuntimeBridge();
    ~Il2CppRuntimeBridge();

    bool Initialize();
    void* GetModuleBase() const { return module_base_; }

    // API Resolvers (Simulated for this implementation, real one uses GetProcAddress/dlsym)
    void* ResolveExport(const std::string& name);

    // IL2CPP Domain API
    void* GetDomain();
    std::vector<void*> GetAssemblies(void* domain);
    void* GetImage(void* assembly);
    std::string GetImageName(void* image);

    // IL2CPP Class API
    size_t GetClassCount(void* image);
    void* GetClass(void* image, size_t index);
    std::string GetClassName(void* klass);
    std::string GetClassNamespace(void* klass);
    
    // IL2CPP Field API
    void* GetFields(void* klass, void** iter);
    std::string GetFieldName(void* field);
    size_t GetFieldOffset(void* field);

private:
    void* module_base_ = nullptr;
    std::map<std::string, void*> api_table_;
};

} // namespace UnityAnalyzer::Runtime
