#pragma once
#include <string>
#include <vector>
#include <cstdint>
#include "../../Unity/Il2CppMetadata/Il2CppMetadata.h"

namespace UnityAnalyzer::IL2CPP::Metadata {

class MetadataReader {
public:
    MetadataReader();
    ~MetadataReader();

    bool Load(const std::string& path);
    bool LoadFromMemory(const uint8_t* buffer, size_t size);

    const UnityAnalyzer::Unity::Il2CppMetadata::Il2CppGlobalMetadataHeader& GetHeader() const { return header_; }
    std::string GetString(int32_t index) const;

    const std::vector<UnityAnalyzer::Unity::Il2CppMetadata::Il2CppImageDefinition>& GetImages() const { return images_; }
    const std::vector<UnityAnalyzer::Unity::Il2CppMetadata::Il2CppTypeDefinition>& GetTypes() const { return types_; }
    const std::vector<UnityAnalyzer::Unity::Il2CppMetadata::Il2CppMethodDefinition>& GetMethods() const { return methods_; }
    const std::vector<UnityAnalyzer::Unity::Il2CppMetadata::Il2CppFieldDefinition>& GetFields() const { return fields_; }
    const std::vector<UnityAnalyzer::Unity::Il2CppMetadata::Il2CppParameterDefinition>& GetParameters() const { return parameters_; }

private:
    std::vector<uint8_t> buffer_;
    UnityAnalyzer::Unity::Il2CppMetadata::Il2CppGlobalMetadataHeader header_;
    const char* stringTable_ = nullptr;

    std::vector<UnityAnalyzer::Unity::Il2CppMetadata::Il2CppImageDefinition> images_;
    std::vector<UnityAnalyzer::Unity::Il2CppMetadata::Il2CppTypeDefinition> types_;
    std::vector<UnityAnalyzer::Unity::Il2CppMetadata::Il2CppMethodDefinition> methods_;
    std::vector<UnityAnalyzer::Unity::Il2CppMetadata::Il2CppFieldDefinition> fields_;
    std::vector<UnityAnalyzer::Unity::Il2CppMetadata::Il2CppParameterDefinition> parameters_;

    bool Parse();
};

} // namespace UnityAnalyzer::IL2CPP::Metadata
