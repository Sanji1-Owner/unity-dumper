#pragma once

#include "Il2CppMetadata.h"
#include <vector>
#include <string>
#include <memory>
#include <map>

namespace UnityAnalyzer::Unity::Il2CppMetadata {

class Il2CppMetadataReader {
public:
    Il2CppMetadataReader();
    ~Il2CppMetadataReader();

    bool Load(const std::string& metadataPath);
    bool LoadFromMemory(const uint8_t* buffer, size_t size);

    int32_t GetVersion() const { return header_.version; }

    std::string GetStringFromIndex(StringIndex index) const;

    const std::vector<Il2CppImageDefinition>& GetImages() const { return images_; }
    const std::vector<Il2CppTypeDefinition>& GetTypes() const { return types_; }
    const std::vector<Il2CppMethodDefinition>& GetMethods() const { return methods_; }
    const std::vector<Il2CppFieldDefinition>& GetFields() const { return fields_; }
    const std::vector<Il2CppParameterDefinition>& GetParameters() const { return parameters_; }

private:
    bool ParseHeader();
    bool ParseImages();
    bool ParseTypes();
    bool ParseMethods();
    bool ParseFields();
    bool ParseParameters();
    bool ParseStrings();

    Il2CppGlobalMetadataHeader header_;
    std::vector<uint8_t> buffer_;
    
    std::vector<Il2CppImageDefinition> images_;
    std::vector<Il2CppTypeDefinition> types_;
    std::vector<Il2CppMethodDefinition> methods_;
    std::vector<Il2CppFieldDefinition> fields_;
    std::vector<Il2CppParameterDefinition> parameters_;
    const char* stringTable_ = nullptr;
};

} // namespace UnityAnalyzer::Unity::Il2CppMetadata
