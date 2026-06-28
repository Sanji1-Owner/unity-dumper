#pragma once

#include <vector>
#include <string>
#include <cstdint>
#include <memory>
#include <map>
#include "../Unity/Objects/ObjectReader.h"

namespace UnityAnalyzer::Archives {

struct TypeNode {
    std::string type;
    std::string name;
    int32_t size;
    uint32_t index;
    int32_t array_flag;
    int32_t flags;
    std::vector<TypeNode> children;
};

struct SerializedType {
    int32_t class_id;
    bool is_stripped_type;
    uint16_t script_type_index;
    std::vector<uint8_t> script_id; // 16 bytes
    std::vector<uint8_t> old_type_hash; // 16 bytes
    TypeNode tree;
};

struct ObjectInfo {
    int64_t path_id;
    uint64_t byte_start;
    uint32_t byte_size;
    int32_t type_id;
    int32_t class_id; // Resolved from type_id
};

class SerializedFile {
public:
    bool Parse(const std::vector<uint8_t>& data);
    const std::vector<ObjectInfo>& GetObjects() const { return objects_; }
    const std::vector<SerializedType>& GetTypes() const { return types_; }
    std::string GetUnityVersion() const { return unity_version_; }
    
    std::vector<uint8_t> GetObjectData(const ObjectInfo& obj, const std::vector<uint8_t>& file_data) const;

private:
    uint32_t header_size_;
    uint32_t file_size_;
    uint32_t version_;
    uint64_t data_offset_;
    uint8_t endian_flag_;
    std::string unity_version_;
    uint32_t target_platform_;
    bool enable_type_tree_;

    std::vector<SerializedType> types_;
    std::vector<ObjectInfo> objects_;

    template<typename T>
    T Read(const std::vector<uint8_t>& data, size_t& offset);
    template<typename T>
    T ReadBE(const std::vector<uint8_t>& data, size_t& offset);
    
    std::string ReadString(const std::vector<uint8_t>& data, size_t& offset);
    void ParseTypeTree(const std::vector<uint8_t>& data, size_t& offset, TypeNode& node);
};

} // namespace UnityAnalyzer::Archives
