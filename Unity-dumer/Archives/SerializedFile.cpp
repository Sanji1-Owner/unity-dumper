#include "SerializedFile.h"
#include <cstring>
#include <algorithm>
#include <iostream>

namespace UnityAnalyzer::Archives {

template<typename T>
T SerializedFile::Read(const std::vector<uint8_t>& data, size_t& offset) {
    T val;
    if (offset + sizeof(T) <= data.size()) {
        std::memcpy(&val, &data[offset], sizeof(T));
        offset += sizeof(T);
    } else {
        std::memset(&val, 0, sizeof(T));
        offset = data.size();
    }
    return val;
}

template<typename T>
T SerializedFile::ReadBE(const std::vector<uint8_t>& data, size_t& offset) {
    if (offset + sizeof(T) > data.size()) {
        offset = data.size();
        return 0;
    }
    T val = Read<T>(data, offset);
    if (sizeof(T) == 4) {
        uint32_t v = *reinterpret_cast<uint32_t*>(&val);
        v = ((v << 24) | ((v << 8) & 0x00FF0000) | ((v >> 8) & 0x0000FF00) | (v >> 24));
        return *reinterpret_cast<T*>(&v);
    }
    return val;
}

std::string SerializedFile::ReadString(const std::vector<uint8_t>& data, size_t& offset) {
    std::string s;
    while (offset < data.size() && data[offset] != '\0') {
        s += (char)data[offset++];
    }
    if (offset < data.size()) offset++;
    return s;
}

void SerializedFile::ParseTypeTree(const std::vector<uint8_t>& data, size_t& offset, TypeNode& node) {
    // Reconstructed tree logic is now inside Parse() for efficiency.
}

bool SerializedFile::Parse(const std::vector<uint8_t>& data) {
    if (data.size() < 20) {
        std::cerr << "SerializedFile: Buffer too small for header\n";
        return false;
    }

    size_t offset = 0;
    header_size_ = ReadBE<uint32_t>(data, offset);
    file_size_ = ReadBE<uint32_t>(data, offset);
    version_ = ReadBE<uint32_t>(data, offset);
    data_offset_ = ReadBE<uint32_t>(data, offset);
    
    if (offset >= data.size()) return false;
    endian_flag_ = data[offset++];
    offset += 3; // padding

    if (version_ >= 7) {
        unity_version_ = ReadString(data, offset);
    }
    if (version_ >= 8) {
        target_platform_ = ReadBE<uint32_t>(data, offset);
    }
    if (version_ >= 13) {
        if (offset < data.size()) enable_type_tree_ = data[offset++] != 0;
    }

    // Types
    uint32_t type_count = Read<uint32_t>(data, offset);
    if (offset > data.size()) return false;
    
    types_.resize(type_count);
    for (uint32_t i = 0; i < type_count; ++i) {
        auto& type = types_[i];
        type.class_id = Read<int32_t>(data, offset);
        if (version_ >= 16) {
             if (offset < data.size()) type.is_stripped_type = data[offset++] != 0;
        }
        if (version_ >= 17) type.script_type_index = Read<uint16_t>(data, offset);
        
        if (type.class_id < 0) {
            if (offset + 16 <= data.size()) {
                type.script_id.assign(data.begin() + offset, data.begin() + offset + 16);
                offset += 16;
            }
        }
        if (offset + 16 <= data.size()) {
            type.old_type_hash.assign(data.begin() + offset, data.begin() + offset + 16);
            offset += 16;
        }

        if (enable_type_tree_) {
            uint32_t node_count = Read<uint32_t>(data, offset);
            uint32_t string_buffer_size = Read<uint32_t>(data, offset);
            
            struct RawNode {
                uint16_t version;
                uint8_t level;
                uint8_t is_array;
                uint32_t type_str_offset;
                uint32_t name_str_offset;
                int32_t byte_size;
                int32_t index;
                uint32_t meta_flags;
            };

            std::vector<RawNode> raw_nodes(node_count);
            for (uint32_t j = 0; j < node_count; ++j) {
                if (offset + 24 > data.size()) return false;
                raw_nodes[j].version = Read<uint16_t>(data, offset);
                raw_nodes[j].level = data[offset++];
                raw_nodes[j].is_array = data[offset++];
                raw_nodes[j].type_str_offset = Read<uint32_t>(data, offset);
                raw_nodes[j].name_str_offset = Read<uint32_t>(data, offset);
                raw_nodes[j].byte_size = Read<int32_t>(data, offset);
                raw_nodes[j].index = Read<int32_t>(data, offset);
                raw_nodes[j].meta_flags = Read<uint32_t>(data, offset);
            }

            if (offset + string_buffer_size > data.size()) return false;
            const char* string_buffer = reinterpret_cast<const char*>(data.data() + offset);
            auto get_string = [&](uint32_t off) -> std::string {
                if (off & 0x80000000) return "CommonType";
                if (off < string_buffer_size) return std::string(string_buffer + off);
                return "Unknown";
            };

            // Reconstruct tree from flat nodes
            std::vector<TypeNode*> stack;
            for (uint32_t j = 0; j < node_count; ++j) {
                TypeNode tn;
                tn.version = raw_nodes[j].version;
                tn.level = raw_nodes[j].level;
                tn.is_array = raw_nodes[j].is_array != 0;
                tn.type = get_string(raw_nodes[j].type_str_offset);
                tn.name = get_string(raw_nodes[j].name_str_offset);
                tn.size = raw_nodes[j].byte_size;
                tn.index = raw_nodes[j].index;
                tn.flags = raw_nodes[j].meta_flags;

                while (stack.size() > tn.level) stack.pop_back();

                if (stack.empty()) {
                    type.tree = tn;
                    stack.push_back(&type.tree);
                } else {
                    stack.back()->children.push_back(tn);
                    stack.push_back(&stack.back()->children.back());
                }
            }
            offset += string_buffer_size;
        }
    }

    // Objects
    uint32_t object_count = Read<uint32_t>(data, offset);
    if (offset > data.size()) return false;
    
    objects_.resize(object_count);
    for (uint32_t i = 0; i < object_count; ++i) {
        if (version_ >= 14) offset = (offset + 3) & ~3;
        auto& obj = objects_[i];
        obj.path_id = (version_ >= 14) ? Read<int64_t>(data, offset) : Read<int32_t>(data, offset);
        obj.byte_start = (version_ >= 22) ? Read<uint64_t>(data, offset) : Read<uint32_t>(data, offset);
        obj.byte_start += data_offset_;
        obj.byte_size = Read<uint32_t>(data, offset);
        obj.type_id = Read<int32_t>(data, offset);
        
        if (obj.type_id >= 0 && (size_t)obj.type_id < types_.size()) {
            obj.class_id = types_[obj.type_id].class_id;
        } else {
            obj.class_id = obj.type_id;
        }
        
        if (version_ >= 11 && version_ < 17) Read<uint16_t>(data, offset);
        if (version_ >= 11 && version_ <= 18) Read<uint16_t>(data, offset);
    }

    return true;
}

std::vector<uint8_t> SerializedFile::GetObjectData(const ObjectInfo& obj, const std::vector<uint8_t>& file_data) const {
    if (obj.byte_start + obj.byte_size <= file_data.size()) {
        return std::vector<uint8_t>(file_data.begin() + obj.byte_start, file_data.begin() + obj.byte_start + obj.byte_size);
    }
    return {};
}

} // namespace UnityAnalyzer::Archives
