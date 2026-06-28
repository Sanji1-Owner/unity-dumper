#include "MetadataAnalyzer.h"

namespace UnityAnalyzer::IL2CPP::Analysis {

MetadataAnalyzer::MetadataAnalyzer(const Metadata::MetadataReader& reader) : reader_(reader) {}

nlohmann::json MetadataAnalyzer::Analyze() {
    nlohmann::json root = nlohmann::json::array();
    const auto& images = reader_.GetImages();
    const auto& types = reader_.GetTypes();
    const auto& methods = reader_.GetMethods();

    for (const auto& image : images) {
        nlohmann::json asmJson;
        asmJson["name"] = reader_.GetString(image.nameIndex);
        nlohmann::json typesJson = nlohmann::json::array();

        for (uint32_t i = 0; i < image.typeCount; ++i) {
            const auto& type = types[image.typeStart + i];
            nlohmann::json t;
            t["name"] = reader_.GetString(type.nameIndex);
            t["namespace"] = reader_.GetString(type.namespaceIndex);
            
            nlohmann::json mList = nlohmann::json::array();
            for (uint16_t m = 0; m < type.methodCount; ++m) {
                const auto& method = methods[type.methodStart + m];
                mList.push_back(reader_.GetString(method.nameIndex));
            }
            t["methods"] = mList;
            typesJson.push_back(t);
        }
        asmJson["types"] = typesJson;
        root.push_back(asmJson);
    }

    return root;
}

} // namespace UnityAnalyzer::IL2CPP::Analysis
