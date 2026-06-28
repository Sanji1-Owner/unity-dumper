#include "Il2CppMetadataReader.h"
#include <fstream>
#include <iostream>
#include <cstring>

namespace UnityAnalyzer::Unity::Il2CppMetadata {

Il2CppMetadataReader::Il2CppMetadataReader() {}

Il2CppMetadataReader::~Il2CppMetadataReader() {}

bool Il2CppMetadataReader::Load(const std::string& metadataPath) {
    std::ifstream file(metadataPath, std::ios::binary | std::ios::ate);
    if (!file.is_open()) return false;

    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    buffer_.resize(size);
    if (!file.read(reinterpret_cast<char*>(buffer_.data()), size)) return false;

    return ParseHeader() && ParseStrings() && ParseImages() && ParseTypes() && ParseMethods() && ParseFields() && ParseParameters();
}

bool Il2CppMetadataReader::LoadFromMemory(const uint8_t* buffer, size_t size) {
    buffer_.assign(buffer, buffer + size);
    return ParseHeader() && ParseStrings() && ParseImages() && ParseTypes() && ParseMethods() && ParseFields() && ParseParameters();
}

bool Il2CppMetadataReader::ParseHeader() {
    if (buffer_.size() < sizeof(Il2CppGlobalMetadataHeader)) return false;
    std::memcpy(&header_, buffer_.data(), sizeof(Il2CppGlobalMetadataHeader));

    if (header_.magic != 0xFAB11BAF) {
        std::cerr << "Invalid metadata magic" << std::endl;
        return false;
    }

    return true;
}

std::string Il2CppMetadataReader::GetStringFromIndex(StringIndex index) const {
    if (!stringTable_ || index < 0 || index >= header_.stringCount) return "";
    return std::string(stringTable_ + index);
}

bool Il2CppMetadataReader::ParseStrings() {
    if (header_.stringOffset + header_.stringCount > buffer_.size()) return false;
    stringTable_ = reinterpret_cast<const char*>(buffer_.data() + header_.stringOffset);
    return true;
}

bool Il2CppMetadataReader::ParseImages() {
    size_t count = header_.imagesCount / sizeof(Il2CppImageDefinition);
    images_.resize(count);
    std::memcpy(images_.data(), buffer_.data() + header_.imagesOffset, header_.imagesCount);
    return true;
}

bool Il2CppMetadataReader::ParseTypes() {
    size_t count = header_.typeDefinitionsCount / sizeof(Il2CppTypeDefinition);
    types_.resize(count);
    std::memcpy(types_.data(), buffer_.data() + header_.typeDefinitionsOffset, header_.typeDefinitionsCount);
    return true;
}

bool Il2CppMetadataReader::ParseMethods() {
    size_t count = header_.methodsCount / sizeof(Il2CppMethodDefinition);
    methods_.resize(count);
    std::memcpy(methods_.data(), buffer_.data() + header_.methodsOffset, header_.methodsCount);
    return true;
}

bool Il2CppMetadataReader::ParseFields() {
    size_t count = header_.fieldsCount / sizeof(Il2CppFieldDefinition);
    fields_.resize(count);
    std::memcpy(fields_.data(), buffer_.data() + header_.fieldsOffset, header_.fieldsCount);
    return true;
}

bool Il2CppMetadataReader::ParseParameters() {
    size_t count = header_.parametersCount / sizeof(Il2CppParameterDefinition);
    parameters_.resize(count);
    std::memcpy(parameters_.data(), buffer_.data() + header_.parametersOffset, header_.parametersCount);
    return true;
}

} // namespace UnityAnalyzer::Unity::Il2CppMetadata
