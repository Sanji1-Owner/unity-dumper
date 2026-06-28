#include "Il2CppAnalyzer.h"
#include "Il2CppMetadata/Il2CppMetadataReader.h"
#include "../Core/ResourceEntry.h"
#include <filesystem>
#include <fstream>
#include <iostream>

namespace UnityAnalyzer::Unity {

bool Il2CppAnalyzer::Initialize(Core::RuntimeContext& context) {
    const auto& info = context.GetRuntimeInfo();
    if (info.backend != Core::ScriptingBackend::IL2CPP) {
        return false;
    }

    // Attempt to locate global-metadata.dat
    // In a real environment, this path is often relative to the executable or Data folder
    metadata_path_ = "global-metadata.dat";
    
    if (!std::filesystem::exists(metadata_path_)) {
        // Search in common locations
        std::vector<std::string> search_paths = {
            "Managed/Metadata/global-metadata.dat",
            "Data/Managed/Metadata/global-metadata.dat",
            "assets/bin/Data/Managed/Metadata/global-metadata.dat"
        };
        for (const auto& p : search_paths) {
            if (std::filesystem::exists(p)) {
                metadata_path_ = p;
                break;
            }
        }
    }

    return std::filesystem::exists(metadata_path_);
}

void Il2CppAnalyzer::ScanMetadata() {
    Il2CppMetadata::Il2CppMetadataReader reader;
    if (!reader.Load(metadata_path_.string())) {
        std::cerr << "Failed to load metadata: " << metadata_path_ << std::endl;
        return;
    }

    const auto& images = reader.GetImages();
    const auto& types = reader.GetTypes();

    for (const auto& image : images) {
        Il2CppAssemblyInfo asmInfo;
        asmInfo.name = reader.GetStringFromIndex(image.nameIndex);

        for (uint32_t i = 0; i < image.typeCount; ++i) {
            const auto& typeDef = types[image.typeStart + i];
            Il2CppTypeInfo typeInfo;
            typeInfo.name = reader.GetStringFromIndex(typeDef.nameIndex);
            typeInfo.namespace_name = reader.GetStringFromIndex(typeDef.namespaceIndex);

            // Extract methods for this type
            for (uint16_t m = 0; m < typeDef.methodCount; ++m) {
                const auto& methodDef = reader.GetMethods()[typeDef.methodStart + m];
                typeInfo.methods.push_back(reader.GetStringFromIndex(methodDef.nameIndex));
            }

            asmInfo.types.push_back(typeInfo);
        }
        assemblies_.push_back(asmInfo);
    }
}

nlohmann::json Il2CppAnalyzer::ExportMetadata() const {
    nlohmann::json json = nlohmann::json::array();
    for (const auto& assembly : assemblies_) {
        nlohmann::json asmJson;
        asmJson["name"] = assembly.name;
        nlohmann::json typesJson = nlohmann::json::array();
        for (const auto& type : assembly.types) {
            nlohmann::json t;
            t["name"] = type.name;
            t["namespace"] = type.namespace_name;
            t["methods"] = type.methods;
            typesJson.push_back(t);
        }
        asmJson["types"] = typesJson;
        json.push_back(asmJson);
    }
    return json;
}

} // namespace UnityAnalyzer::Unity
