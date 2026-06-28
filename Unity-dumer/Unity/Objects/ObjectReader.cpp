#include "ObjectReader.h"
#include "../../Core/Logger.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <cstring>
#include <iomanip>
#include <filesystem>
#include <vector>

namespace UnityAnalyzer::Unity::Objects {

template<typename T>
bool SafeReadVal(const std::vector<uint8_t>& data, size_t& offset, T& out) {
    if (offset + sizeof(T) > data.size()) {
        Core::Logger::Instance().Error("SafeReadVal: Out of bounds read at offset " + std::to_string(offset));
        std::memset(&out, 0, sizeof(T));
        return false;
    }
    std::memcpy(&out, &data[offset], sizeof(T));
    offset += sizeof(T);
    return true;
}

template<typename T>
T ReadVal(const std::vector<uint8_t>& data, size_t& offset) {
    T val;
    SafeReadVal(data, offset, val);
    return val;
}

std::string SafeReadStr(const std::vector<uint8_t>& data, size_t& offset) {
    if (offset + 4 > data.size()) {
        Core::Logger::Instance().Error("SafeReadStr: Header out of bounds");
        return "";
    }
    uint32_t len = *reinterpret_cast<const uint32_t*>(&data[offset]);
    offset += 4;
    if (offset + len > data.size()) {
        Core::Logger::Instance().Error("SafeReadStr: String data out of bounds (len: " + std::to_string(len) + ")");
        return "";
    }
    std::string s(reinterpret_cast<const char*>(&data[offset]), len);
    offset += (len + 3) & ~3;
    return s;
}

std::string ReadStr(const std::vector<uint8_t>& data, size_t& offset) {
    return SafeReadStr(data, offset);
}

// Texture2D
bool Texture2DReader::Parse(const SerializedObject& obj, Core::ResourceEntry& entry) {
    entry.resource_type = Core::ResourceType::Texture;
    size_t offset = 0;
    entry.name = SafeReadStr(obj.data, offset);
    
    int32_t forcedFallback;
    if (!SafeReadVal(obj.data, offset, forcedFallback)) return false;
    bool downscale;
    if (!SafeReadVal(obj.data, offset, downscale)) return false;
    offset = (offset + 3) & ~3;
    
    uint32_t width, height, complete_image_size, texture_format;
    if (!SafeReadVal(obj.data, offset, width)) return false;
    if (!SafeReadVal(obj.data, offset, height)) return false;
    if (!SafeReadVal(obj.data, offset, complete_image_size)) return false;
    if (!SafeReadVal(obj.data, offset, texture_format)) return false;
    
    entry.metadata["width"] = std::to_string(width);
    entry.metadata["height"] = std::to_string(height);
    entry.metadata["format"] = std::to_string(texture_format);
    entry.metadata["size"] = std::to_string(complete_image_size);
    entry.metadata["data_offset"] = std::to_string(offset);
    
    return true;
}

bool Texture2DReader::Export(const Core::ResourceEntry& entry, const std::vector<uint8_t>& data, const std::filesystem::path& target_path) {
    try {
        uint32_t format = std::stoul(entry.metadata.at("format"));
        uint32_t width = std::stoul(entry.metadata.at("width"));
        uint32_t height = std::stoul(entry.metadata.at("height"));
        size_t data_offset = std::stoul(entry.metadata.at("data_offset"));

        std::string ext = ".raw";
        if (format == 10 || format == 12 || format == 28 || format == 29) ext = ".dds";
        else if (format == 3 || format == 4 || format == 5) ext = ".tga";

        std::ofstream out(target_path.string() + ext, std::ios::binary);
        if (!out) return false;

        if (ext == ".dds") {
            uint32_t dwMagic = 0x20534444;
            out.write(reinterpret_cast<const char*>(&dwMagic), 4);
            uint8_t header[124] = {0};
            header[0] = 124;
            header[1] = 0x01 | 0x02 | 0x04 | 0x1000 | 0x00080000;
            *reinterpret_cast<uint32_t*>(&header[8]) = height;
            *reinterpret_cast<uint32_t*>(&header[12]) = width;
            header[72] = 32;
            header[76] = 0x04;
            const char* fourCC = (format == 10 || format == 28) ? "DXT1" : "DXT5";
            std::memcpy(&header[80], fourCC, 4);
            out.write(reinterpret_cast<const char*>(header), 124);
        } else if (ext == ".tga") {
            uint8_t header[18] = {0};
            header[2] = 2;
            *reinterpret_cast<uint16_t*>(&header[12]) = (uint16_t)width;
            *reinterpret_cast<uint16_t*>(&header[14]) = (uint16_t)height;
            header[16] = (format == 3) ? 24 : 32;
            out.write(reinterpret_cast<const char*>(header), 18);
        }

        if (data_offset < data.size()) {
            size_t remaining = data.size() - data_offset;
            out.write(reinterpret_cast<const char*>(&data[data_offset]), remaining);
        }
        return true;
    } catch (const std::exception& e) {
        Core::Logger::Instance().Error("Texture2D Export failed: " + std::string(e.what()));
        return false;
    }
}

// Mesh
bool MeshReader::Parse(const SerializedObject& obj, Core::ResourceEntry& entry) {
    entry.resource_type = Core::ResourceType::Mesh;
    size_t offset = 0;
    entry.name = SafeReadStr(obj.data, offset);
    
    uint32_t submesh_count;
    if (!SafeReadVal(obj.data, offset, submesh_count)) return false;
    entry.metadata["submesh_count"] = std::to_string(submesh_count);
    for(uint32_t i=0; i<submesh_count; ++i) {
        if (offset + 24 > obj.data.size()) return false;
        offset += 24; 
    }
    
    uint32_t index_format, index_buffer_size;
    if (!SafeReadVal(obj.data, offset, index_format)) return false;
    if (!SafeReadVal(obj.data, offset, index_buffer_size)) return false;
    size_t ib_offset = offset;
    offset += (index_buffer_size + 3) & ~3;
    
    if (offset + 8 > obj.data.size()) return false;
    uint32_t current_channels, vertex_count;
    if (!SafeReadVal(obj.data, offset, current_channels)) return false;
    if (!SafeReadVal(obj.data, offset, vertex_count)) return false;
    
    entry.metadata["vertex_count"] = std::to_string(vertex_count);
    entry.metadata["index_format"] = std::to_string(index_format);
    entry.metadata["index_count"] = std::to_string(index_buffer_size / (index_format == 0 ? 2 : 4));
    entry.metadata["ib_offset"] = std::to_string(ib_offset);
    entry.metadata["ib_size"] = std::to_string(index_buffer_size);
    entry.metadata["vb_offset"] = std::to_string(offset + (current_channels * 12));
    
    return true;
}

bool MeshReader::Export(const Core::ResourceEntry& entry, const std::vector<uint8_t>& data, const std::filesystem::path& target_path) {
    try {
        std::ofstream out(target_path.string() + ".obj");
        if (!out) return false;
        out << "# UnityAnalyzer Mesh Export: " << entry.name << "\n";
        
        uint32_t vertex_count = std::stoul(entry.metadata.at("vertex_count"));
        size_t vb_offset = std::stoul(entry.metadata.at("vb_offset"));
        size_t ib_offset = std::stoul(entry.metadata.at("ib_offset"));
        uint32_t ib_size = std::stoul(entry.metadata.at("ib_size"));
        uint32_t index_format = std::stoul(entry.metadata.at("index_format"));
        
        if (vb_offset + (vertex_count * 12) <= data.size()) {
            for (uint32_t i = 0; i < vertex_count; ++i) {
                float x, y, z;
                std::memcpy(&x, &data[vb_offset + i * 12], 4);
                std::memcpy(&y, &data[vb_offset + i * 12 + 4], 4);
                std::memcpy(&z, &data[vb_offset + i * 12 + 8], 4);
                out << "v " << x << " " << y << " " << z << "\n";
            }
        }
        
        if (ib_offset + ib_size <= data.size()) {
            uint32_t stride = (index_format == 0 ? 2 : 4);
            for (uint32_t i = 0; i < ib_size; i += stride * 3) {
                if (i + stride * 2 >= ib_size) break;
                uint32_t i1, i2, i3;
                if (stride == 2) {
                    i1 = *reinterpret_cast<const uint16_t*>(&data[ib_offset + i]);
                    i2 = *reinterpret_cast<const uint16_t*>(&data[ib_offset + i + 2]);
                    i3 = *reinterpret_cast<const uint16_t*>(&data[ib_offset + i + 4]);
                } else {
                    i1 = *reinterpret_cast<const uint32_t*>(&data[ib_offset + i]);
                    i2 = *reinterpret_cast<const uint32_t*>(&data[ib_offset + i + 4]);
                    i3 = *reinterpret_cast<const uint32_t*>(&data[ib_offset + i + 8]);
                }
                out << "f " << (i1 + 1) << " " << (i2 + 1) << " " << (i3 + 1) << "\n";
            }
        }
        return true;
    } catch (const std::exception& e) {
        Core::Logger::Instance().Error("Mesh Export failed: " + std::string(e.what()));
        return false;
    }
}

// AudioClip
bool AudioClipReader::Parse(const SerializedObject& obj, Core::ResourceEntry& entry) {
    entry.resource_type = Core::ResourceType::AudioClip;
    size_t offset = 0;
    entry.name = SafeReadStr(obj.data, offset);
    
    uint32_t load_type;
    SafeReadVal(obj.data, offset, load_type);
    bool is_tracker, ambisonic;
    SafeReadVal(obj.data, offset, is_tracker);
    SafeReadVal(obj.data, offset, ambisonic);
    offset = (offset + 3) & ~3;
    
    uint32_t subsound_index;
    SafeReadVal(obj.data, offset, subsound_index);
    bool preload, background, legacy3d;
    SafeReadVal(obj.data, offset, preload);
    SafeReadVal(obj.data, offset, background);
    SafeReadVal(obj.data, offset, legacy3d);
    offset = (offset + 3) & ~3;
    
    std::string source = SafeReadStr(obj.data, offset);
    uint64_t res_offset, res_size;
    SafeReadVal(obj.data, offset, res_offset);
    SafeReadVal(obj.data, offset, res_size);
    
    uint32_t compression_format;
    SafeReadVal(obj.data, offset, compression_format);

    // Audio metadata usually follows
    uint32_t frequency, channels;
    SafeReadVal(obj.data, offset, frequency);
    SafeReadVal(obj.data, offset, channels);
    
    entry.metadata["channels"] = std::to_string(channels > 0 ? channels : 2);
    entry.metadata["frequency"] = std::to_string(frequency > 0 ? frequency : 44100);
    entry.metadata["data_offset"] = std::to_string(offset);
    entry.metadata["data_size"] = std::to_string(res_size > 0 ? (size_t)res_size : (obj.data.size() - offset));
    
    return true;
}

bool AudioClipReader::Export(const Core::ResourceEntry& entry, const std::vector<uint8_t>& data, const std::filesystem::path& target_path) {
    try {
        size_t data_offset = std::stoul(entry.metadata.at("data_offset"));
        uint32_t data_size = (uint32_t)std::stoul(entry.metadata.at("data_size"));
        uint32_t channels = std::stoul(entry.metadata.at("channels"));
        uint32_t frequency = std::stoul(entry.metadata.at("frequency"));

        std::ofstream out(target_path.string() + ".wav", std::ios::binary);
        if (!out) return false;

        out.write("RIFF", 4);
        uint32_t chunk_size = 36 + data_size;
        out.write(reinterpret_cast<const char*>(&chunk_size), 4);
        out.write("WAVE", 4);
        
        out.write("fmt ", 4);
        uint32_t subchunk1_size = 16;
        out.write(reinterpret_cast<const char*>(&subchunk1_size), 4);
        uint16_t audio_format = 1; // PCM
        out.write(reinterpret_cast<const char*>(&audio_format), 2);
        uint16_t num_channels = (uint16_t)channels;
        out.write(reinterpret_cast<const char*>(&num_channels), 2);
        out.write(reinterpret_cast<const char*>(&frequency), 4);
        uint32_t byte_rate = frequency * channels * 2;
        out.write(reinterpret_cast<const char*>(&byte_rate), 4);
        uint16_t block_align = (uint16_t)(channels * 2);
        out.write(reinterpret_cast<const char*>(&block_align), 2);
        uint16_t bits_per_sample = 16;
        out.write(reinterpret_cast<const char*>(&bits_per_sample), 2);
        
        out.write("data", 4);
        out.write(reinterpret_cast<const char*>(&data_size), 4);
        if (data_offset + data_size <= data.size()) {
            out.write(reinterpret_cast<const char*>(&data[data_offset]), data_size);
        }
        return true;
    } catch (const std::exception& e) {
        Core::Logger::Instance().Error("AudioClip Export failed: " + std::string(e.what()));
        return false;
    }
}

// Transform
bool TransformReader::Parse(const SerializedObject& obj, Core::ResourceEntry& entry) {
    entry.resource_type = Core::ResourceType::Unknown;
    size_t offset = 0;
    int32_t game_object_id;
    if (!SafeReadVal(obj.data, offset, game_object_id)) return false;
    entry.metadata["game_object"] = std::to_string(game_object_id);
    
    float px, py, pz;
    SafeReadVal(obj.data, offset, px);
    SafeReadVal(obj.data, offset, py);
    SafeReadVal(obj.data, offset, pz);
    entry.metadata["pos"] = std::to_string(px) + "," + std::to_string(py) + "," + std::to_string(pz);
    
    int32_t parent_id;
    SafeReadVal(obj.data, offset, parent_id);
    entry.metadata["parent"] = std::to_string(parent_id);
    
    uint32_t children_count;
    SafeReadVal(obj.data, offset, children_count);
    std::string children;
    for(uint32_t i=0; i<children_count; ++i) {
        int32_t child_id;
        if (!SafeReadVal(obj.data, offset, child_id)) break;
        children += std::to_string(child_id) + ",";
    }
    entry.metadata["children"] = children;

    return true;
}

bool TransformReader::Export(const Core::ResourceEntry& entry, const std::vector<uint8_t>& data, const std::filesystem::path& target_path) {
    return true;
}

// GameObject
bool GameObjectReader::Parse(const SerializedObject& obj, Core::ResourceEntry& entry) {
    entry.resource_type = Core::ResourceType::GameObject;
    size_t offset = 0;
    uint32_t component_count;
    if (!SafeReadVal(obj.data, offset, component_count)) return false;
    std::string components;
    for(uint32_t i=0; i<component_count; ++i) {
        int32_t class_id;
        SafeReadVal(obj.data, offset, class_id);
        int64_t path_id;
        if (obj.data.size() > 40) {
            SafeReadVal(obj.data, offset, path_id);
        } else {
            int32_t path_id32;
            SafeReadVal(obj.data, offset, path_id32);
            path_id = path_id32;
        }
        components += std::to_string(class_id) + ":" + std::to_string(path_id) + ";";
    }
    entry.metadata["components"] = components;
    entry.name = SafeReadStr(obj.data, offset);
    return true;
}

bool GameObjectReader::Export(const Core::ResourceEntry& entry, const std::vector<uint8_t>& data, const std::filesystem::path& target_path) {
    return true;
}

// MonoBehaviour
bool MonoBehaviourReader::Parse(const SerializedObject& obj, Core::ResourceEntry& entry) {
    entry.resource_type = Core::ResourceType::MonoBehaviour;
    size_t offset = 0;
    int32_t go_id;
    SafeReadVal(obj.data, offset, go_id);
    uint8_t enabled;
    SafeReadVal(obj.data, offset, enabled);
    offset = (offset + 3) & ~3;
    int32_t script_id;
    SafeReadVal(obj.data, offset, script_id);
    entry.metadata["script_id"] = std::to_string(script_id);
    entry.name = SafeReadStr(obj.data, offset);
    return true;
}
bool MonoBehaviourReader::Export(const Core::ResourceEntry& entry, const std::vector<uint8_t>& data, const std::filesystem::path& target_path) { return true; }

// TextAsset
bool TextAssetReader::Parse(const SerializedObject& obj, Core::ResourceEntry& entry) {
    size_t offset = 0;
    entry.name = SafeReadStr(obj.data, offset);
    entry.metadata["data_offset"] = std::to_string(offset);
    return true;
}
bool TextAssetReader::Export(const Core::ResourceEntry& entry, const std::vector<uint8_t>& data, const std::filesystem::path& target_path) {
    try {
        size_t data_offset = std::stoul(entry.metadata.at("data_offset"));
        std::ofstream out(target_path.string() + ".txt", std::ios::binary);
        if (data_offset < data.size()) out.write(reinterpret_cast<const char*>(&data[data_offset]), data.size() - data_offset);
        return true;
    } catch (...) { return false; }
}

// AnimationClip (Partial)
bool AnimationClipReader::Parse(const SerializedObject& obj, Core::ResourceEntry& entry) {
    entry.resource_type = Core::ResourceType::AnimationClip;
    size_t offset = 0;
    entry.name = SafeReadStr(obj.data, offset);
    entry.metadata["status"] = "Partial";
    entry.metadata["info"] = "Curve extraction requires complex TypeTree parsing not fully implemented.";
    return true;
}
bool AnimationClipReader::Export(const Core::ResourceEntry& entry, const std::vector<uint8_t>& data, const std::filesystem::path& target_path) {
    std::ofstream out(target_path.string() + ".anim.json");
    out << "{\n  \"name\": \"" << entry.name << "\",\n  \"status\": \"Partial\",\n  \"note\": \"Metadata only\"\n}\n";
    return true;
}

// Shader (Partial)
bool ShaderReader::Parse(const SerializedObject& obj, Core::ResourceEntry& entry) {
    size_t offset = 0;
    entry.name = SafeReadStr(obj.data, offset);
    entry.metadata["status"] = "Partial";
    entry.metadata["data_offset"] = std::to_string(offset);
    return true;
}
bool ShaderReader::Export(const Core::ResourceEntry& entry, const std::vector<uint8_t>& data, const std::filesystem::path& target_path) {
    try {
        size_t data_offset = std::stoul(entry.metadata.at("data_offset"));
        std::ofstream out(target_path.string() + ".shader", std::ios::binary);
        if (data_offset < data.size()) out.write(reinterpret_cast<const char*>(&data[data_offset]), data.size() - data_offset);
        return true;
    } catch (...) { return false; }
}

// Sprite (Partial)
bool SpriteReader::Parse(const SerializedObject& obj, Core::ResourceEntry& entry) {
    entry.resource_type = Core::ResourceType::Sprite;
    size_t offset = 0;
    entry.name = SafeReadStr(obj.data, offset);
    return true;
}
bool SpriteReader::Export(const Core::ResourceEntry& entry, const std::vector<uint8_t>& data, const std::filesystem::path& target_path) {
    std::ofstream out(target_path.string() + ".sprite.json");
    out << "{\n  \"name\": \"" << entry.name << "\",\n  \"status\": \"Partial\"\n}\n";
    return true;
}

std::unique_ptr<ObjectReader> ObjectReaderFactory::CreateReader(const std::string& type_name) {
    if (type_name == "Texture2D") return std::make_unique<Texture2DReader>();
    if (type_name == "Sprite") return std::make_unique<SpriteReader>();
    if (type_name == "Mesh") return std::make_unique<MeshReader>();
    if (type_name == "AudioClip") return std::make_unique<AudioClipReader>();
    if (type_name == "Material") return std::make_unique<MaterialReader>();
    if (type_name == "GameObject") return std::make_unique<GameObjectReader>();
    if (type_name == "MonoBehaviour") return std::make_unique<MonoBehaviourReader>();
    if (type_name == "TextAsset") return std::make_unique<TextAssetReader>();
    if (type_name == "AnimationClip") return std::make_unique<AnimationClipReader>();
    if (type_name == "Shader") return std::make_unique<ShaderReader>();
    if (type_name == "Transform") return std::make_unique<TransformReader>();
    return nullptr;
}

std::unique_ptr<ObjectReader> ObjectReaderFactory::CreateReaderByClassId(uint32_t class_id) {
    switch (class_id) {
        case 28:  return std::make_unique<Texture2DReader>();
        case 213: return std::make_unique<SpriteReader>();
        case 43:  return std::make_unique<MeshReader>();
        case 83:  return std::make_unique<AudioClipReader>();
        case 21:  return std::make_unique<MaterialReader>();
        case 1:   return std::make_unique<GameObjectReader>();
        case 4:   return std::make_unique<TransformReader>();
        case 114: return std::make_unique<MonoBehaviourReader>();
        case 49:  return std::make_unique<TextAssetReader>();
        case 74:  return std::make_unique<AnimationClipReader>();
        case 48:  return std::make_unique<ShaderReader>();
        default:  return nullptr;
    }
}

} // namespace UnityAnalyzer::Unity::Objects
