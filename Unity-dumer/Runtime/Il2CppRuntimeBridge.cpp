#include "Il2CppRuntimeBridge.h"
#include <iostream>

#ifdef _WIN32
#include <windows.h>
#else
#include <dlfcn.h>
#endif

namespace UnityAnalyzer::Runtime {

Il2CppRuntimeBridge::Il2CppRuntimeBridge() {}
Il2CppRuntimeBridge::~Il2CppRuntimeBridge() {}

bool Il2CppRuntimeBridge::Initialize() {
#ifdef _WIN32
    module_base_ = GetModuleHandleA("GameAssembly.dll");
    if (!module_base_) module_base_ = GetModuleHandleA("UnityPlayer.dll");
#else
    module_base_ = dlopen("libil2cpp.so", RTLD_LAZY);
#endif
    return module_base_ != nullptr;
}

void* Il2CppRuntimeBridge::ResolveExport(const std::string& name) {
    if (api_table_.count(name)) return api_table_[name];
#ifdef _WIN32
    void* addr = (void*)GetProcAddress((HMODULE)module_base_, name.c_str());
#else
    void* addr = dlsym(module_base_, name.c_str());
#endif
    api_table_[name] = addr;
    return addr;
}

void* Il2CppRuntimeBridge::GetDomain() {
    auto func = (void* (*)())ResolveExport("il2cpp_domain_get");
    return func ? func() : nullptr;
}

std::vector<void*> Il2CppRuntimeBridge::GetAssemblies(void* domain) {
    std::vector<void*> assemblies;
    auto func = (void** (*)(void*, size_t*))ResolveExport("il2cpp_domain_get_assemblies");
    if (func) {
        size_t size = 0;
        void** list = func(domain, &size);
        for (size_t i = 0; i < size; ++i) assemblies.push_back(list[i]);
    }
    return assemblies;
}

void* Il2CppRuntimeBridge::GetImage(void* assembly) {
    auto func = (void* (*)(void*))ResolveExport("il2cpp_assembly_get_image");
    return func ? func(assembly) : nullptr;
}

std::string Il2CppRuntimeBridge::GetImageName(void* image) {
    auto func = (const char* (*)(void*))ResolveExport("il2cpp_image_get_name");
    return func ? func(image) : "";
}

size_t Il2CppRuntimeBridge::GetClassCount(void* image) {
    auto func = (size_t (*)(void*))ResolveExport("il2cpp_image_get_class_count");
    return func ? func(image) : 0;
}

void* Il2CppRuntimeBridge::GetClass(void* image, size_t index) {
    auto func = (void* (*)(void*, size_t))ResolveExport("il2cpp_image_get_class");
    return func ? func(image, index) : nullptr;
}

std::string Il2CppRuntimeBridge::GetClassName(void* klass) {
    auto func = (const char* (*)(void*))ResolveExport("il2cpp_class_get_name");
    return func ? func(klass) : "";
}

std::string Il2CppRuntimeBridge::GetClassNamespace(void* klass) {
    auto func = (const char* (*)(void*))ResolveExport("il2cpp_class_get_namespace");
    return func ? func(klass) : "";
}

void* Il2CppRuntimeBridge::GetFields(void* klass, void** iter) {
    auto func = (void* (*)(void*, void**))ResolveExport("il2cpp_class_get_fields");
    return func ? func(klass, iter) : nullptr;
}

std::string Il2CppRuntimeBridge::GetFieldName(void* field) {
    auto func = (const char* (*)(void*))ResolveExport("il2cpp_field_get_name");
    return func ? func(field) : "";
}

size_t Il2CppRuntimeBridge::GetFieldOffset(void* field) {
    auto func = (size_t (*)(void*))ResolveExport("il2cpp_field_get_offset");
    return func ? func(field) : 0;
}

} // namespace UnityAnalyzer::Runtime
