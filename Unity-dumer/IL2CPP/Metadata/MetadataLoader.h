#pragma once
#include <string>
#include <vector>
#include <cstdint>
#include "../../Unity/Il2CppMetadata/Il2CppMetadata.h"

namespace UnityAnalyzer::IL2CPP::Metadata {

class MetadataLoader {
public:
    MetadataLoader();
    ~MetadataLoader();

    bool Load(const std::string& path);
    bool LoadFromMemory(const uint8_t* buffer, size_t size);

    const UnityAnalyzer::Unity::Il2CppMetadata::Il2CppGlobalMetadataHeader& GetHeader() const { return header_; }
    const char* GetString(int32_t index) const;

private:
    std::vector<uint8_t> buffer_;
    UnityAnalyzer::Unity::Il2CppMetadata::Il2CppGlobalMetadataHeader header_;
    const char* stringTable_ = nullptr;

    bool Parse();
};

} // namespace UnityAnalyzer::IL2CPP::Metadata
