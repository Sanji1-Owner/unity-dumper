#include <iostream>
#include <fstream>
#include <vector>
#include <cassert>
#include <filesystem>
#include "../UnityAnalyzer/Unity/Objects/ObjectReader.h"

using namespace UnityAnalyzer::Unity::Objects;

void TestMeshReal() {
    std::cout << "Running MeshRealValidation..." << std::endl;

    SerializedObject obj;
    obj.class_id = 43;
    obj.path_id = 1;
    
    // Fake Mesh Data: Name(len 4 + "Cube" + padding) + SubmeshCount(0) + IndexFormat(0) + IndexBufferSize(6) + Indices(1,2,3) + Channels(1) + VertexCount(3) + Vertices(float3 x 3)
    std::vector<uint8_t> data = {
        4, 0, 0, 0, 'C', 'u', 'b', 'e',
        0, 0, 0, 0, // Submeshes
        0, 0, 0, 0, // IndexFormat
        6, 0, 0, 0, // IndexBufferSize
        0, 0, 1, 0, 2, 0, // Indices: 0, 1, 2
        0, 0, // padding
        1, 0, 0, 0, // Channels
        3, 0, 0, 0  // VertexCount
    };
    
    // Append 3 vertices (float3 each)
    float v1[] = {0,0,0, 1,0,0, 0,1,0};
    uint8_t* ptr = reinterpret_cast<uint8_t*>(v1);
    data.insert(data.end(), ptr, ptr + sizeof(v1));
    
    obj.data = data;

    MeshReader reader;
    UnityAnalyzer::Core::ResourceEntry entry;
    assert(reader.Parse(obj, entry) == true);
    assert(entry.name == "Cube");
    assert(entry.metadata["vertex_count"] == "3");
    
    std::filesystem::path test_path = "test_mesh";
    assert(reader.Export(entry, obj.data, test_path) == true);
    
    std::ifstream obj_file("test_mesh.obj");
    assert(obj_file.is_open());
    
    std::string line;
    int v_count = 0, f_count = 0;
    while(std::getline(obj_file, line)) {
        if(line.substr(0, 2) == "v ") v_count++;
        if(line.substr(0, 2) == "f ") f_count++;
    }
    
    assert(v_count == 3);
    assert(f_count == 1);
    
    std::cout << "MeshRealValidation: PASS" << std::endl;
}

int main() {
    TestMeshReal();
    return 0;
}
