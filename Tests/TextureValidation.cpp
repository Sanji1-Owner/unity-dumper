#include <iostream>
#include <fstream>
#include <vector>
#include <cassert>
#include <cstring>
#include <filesystem>
#include "../UnityAnalyzer/Unity/Objects/ObjectReader.h"

using namespace UnityAnalyzer::Unity::Objects;

void TestTextureValidation() {
    std::cout << "Running TextureValidation..." << std::endl;

    SerializedObject obj;
    obj.class_id = 28;
    obj.path_id = 3;
    
    // Fake Texture2D Data: Name(len 4 + "Tex" + pad) + ForcedFallback(0) + Downscale(0) + pad + Width(2) + Height(2) + Size(16) + Format(10 - DXT1)
    std::vector<uint8_t> data = {
        3, 0, 0, 0, 'T', 'e', 'x', 0,
        0, 0, 0, 0, // ForcedFallback
        0, 0, 0, 0, // Downscale + padding
        2, 0, 0, 0, // Width
        2, 0, 0, 0, // Height
        16, 0, 0, 0, // Size
        10, 0, 0, 0 // Format (DXT1)
    };
    // Append 16 bytes of dummy DXT1 data
    data.resize(data.size() + 16, 0xCC);
    
    obj.data = data;

    Texture2DReader reader;
    UnityAnalyzer::Core::ResourceEntry entry;
    assert(reader.Parse(obj, entry) == true);
    assert(entry.name == "Tex");
    assert(entry.metadata["format"] == "10");
    
    std::filesystem::path test_path = "test_tex";
    assert(reader.Export(entry, obj.data, test_path) == true);
    
    std::ifstream dds_file("test_tex.dds", std::ios::binary);
    assert(dds_file.is_open());
    
    uint32_t magic;
    dds_file.read(reinterpret_cast<char*>(&magic), 4);
    assert(magic == 0x20534444); // 'DDS '
    
    std::cout << "TextureValidation: PASS" << std::endl;
}

int main() {
    TestTextureValidation();
    return 0;
}
