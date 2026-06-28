#include "SDKExporter.h"
#include <fstream>
#include <iostream>

namespace UnityAnalyzer::IL2CPP::Export {

SDKExporter::SDKExporter(const Metadata::MetadataReader& reader) : reader_(reader) {}

bool SDKExporter::Export(const std::filesystem::path& outputDir) {
    std::filesystem::create_directories(outputDir);
    const auto& images = reader_.GetImages();
    
    for (size_t i = 0; i < images.size(); ++i) {
        std::string name = reader_.GetString(images[i].nameIndex);
        // Remove .dll for directory name
        if (name.size() > 4 && name.substr(name.size() - 4) == ".dll") {
            name = name.substr(0, name.size() - 4);
        }
        std::filesystem::path assemblyDir = outputDir / name;
        std::filesystem::create_directories(assemblyDir);
        ExportAssembly(assemblyDir, static_cast<int32_t>(i));
    }
    return true;
}

void SDKExporter::ExportAssembly(const std::filesystem::path& assemblyDir, int32_t imageIndex) {
    const auto& image = reader_.GetImages()[imageIndex];
    const auto& types = reader_.GetTypes();
    const auto& methods = reader_.GetMethods();
    const auto& fields = reader_.GetFields();
    
    std::ofstream classesFile(assemblyDir / "Classes.cs");
    
    for (uint32_t i = 0; i < image.typeCount; ++i) {
        const auto& type = types[image.typeStart + i];
        std::string ns = reader_.GetString(type.namespaceIndex);
        std::string name = reader_.GetString(type.nameIndex);
        
        if (!ns.empty()) {
            classesFile << "namespace " << ns << " {\n";
        }
        
        classesFile << "  public class " << name << " { // Token: 0x" << std::hex << type.token << std::dec << "\n";
        
        // Fields
        for (uint16_t f = 0; f < type.fieldCount; ++f) {
            const auto& field = fields[type.fieldStart + f];
            classesFile << "    public object " << reader_.GetString(field.nameIndex) << "; // Token: 0x" << std::hex << field.token << std::dec << "\n";
        }
        
        // Methods
        for (uint16_t m = 0; m < type.methodCount; ++m) {
            const auto& method = methods[type.methodStart + m];
            classesFile << "    public void " << reader_.GetString(method.nameIndex) << "(); // Token: 0x" << std::hex << method.token << std::dec << "\n";
        }
        
        classesFile << "  }\n";
        
        if (!ns.empty()) {
            classesFile << "}\n";
        }
        classesFile << "\n";
    }
}

} // namespace UnityAnalyzer::IL2CPP::Export
