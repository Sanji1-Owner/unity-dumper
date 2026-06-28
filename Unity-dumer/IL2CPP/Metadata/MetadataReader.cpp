#pragma once
#include "MetadataReader.h"
#include <fstream>
#include <iostream>
#include <cstring>

namespace UnityAnalyzer::IL2CPP::Metadata {

MetadataReader::MetadataReader() {}

MetadataReader::~MetadataReader() {}

bool MetadataReader::Load(const std::string& path) {
    std::ifstream file(path, std::ios::binary | std::ios::ate);
    if (!file.is_open()) return false;

    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    buffer_.resize(size);
    if (!file.read(reinterpret_cast<char*>(buffer_.data()), size)) return false;

    return Parse();
}

bool MetadataReader::LoadFromMemory(const uint8_t* buffer, size_t size) {
    buffer_.assign(buffer, buffer + size);
    return Parse();
}

bool MetadataReader::Parse() {
    if (buffer_.size() < sizeof(UnityAnalyzer::Unity::Il2CppMetadata::Il2CppGlobalMetadataHeader)) return false;
    
    std::memcpy(&header_, buffer_.data(), sizeof(UnityAnalyzer::Unity::Il2CppMetadata::Il2CppGlobalMetadataHeader));

    if (header_.magic != 0xFAB11BAF) {
        return false;
    }

    // String Table
    if (header_.stringOffset + header_.stringCount <= buffer_.size()) {
        stringTable_ = reinterpret_cast<const char*>(buffer_.data() + header_.stringOffset);
    }

    // Images
    size_t imagesCount = header_.imagesCount / sizeof(UnityAnalyzer::Unity::Il2CppMetadata::Il2CppImageDefinition);
    if (header_.imagesOffset + header_.imagesCount <= buffer_.size()) {
        images_.resize(imagesCount);
        std::memcpy(images_.data(), buffer_.data() + header_.imagesOffset, header_.imagesCount);
    }

    // Types
    size_t typesCount = header_.typeDefinitionsCount / sizeof(UnityAnalyzer::Unity::Il2CppMetadata::Il2CppTypeDefinition);
    if (header_.typeDefinitionsOffset + header_.typeDefinitionsCount <= buffer_.size()) {
        types_.resize(typesCount);
        std::memcpy(types_.data(), buffer_.data() + header_.typeDefinitionsOffset, header_.typeDefinitionsCount);
    }

    // Methods
    size_t methodsCount = header_.methodsCount / sizeof(UnityAnalyzer::Unity::Il2CppMetadata::Il2CppMethodDefinition);
    if (header_.methodsOffset + header_.methodsCount <= buffer_.size()) {
        methods_.resize(methodsCount);
        std::memcpy(methods_.data(), buffer_.data() + header_.methodsOffset, header_.methodsCount);
    }

    // Fields
    size_t fieldsCount = header_.fieldsCount / sizeof(UnityAnalyzer::Unity::Il2CppMetadata::Il2CppFieldDefinition);
    if (header_.fieldsOffset + header_.fieldsCount <= buffer_.size()) {
        fields_.resize(fieldsCount);
        std::memcpy(fields_.data(), buffer_.data() + header_.fieldsOffset, header_.fieldsCount);
    }

    // Parameters
    size_t paramsCount = header_.parametersCount / sizeof(UnityAnalyzer::Unity::Il2CppMetadata::Il2CppParameterDefinition);
    if (header_.parametersOffset + header_.parametersCount <= buffer_.size()) {
        parameters_.resize(paramsCount);
        std::memcpy(parameters_.data(), buffer_.data() + header_.parametersOffset, header_.parametersCount);
    }

    return true;
}

std::string MetadataReader::GetString(int32_t index) const {
    if (!stringTable_ || index < 0 || index >= header_.stringCount) return "";
    return std::string(stringTable_ + index);
}

} // namespace UnityAnalyzer::IL2CPP::Metadata
