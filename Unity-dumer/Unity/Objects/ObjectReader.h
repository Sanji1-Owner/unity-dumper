#pragma once

#include "../../Core/ResourceEntry.h"
#include <vector>
#include <cstdint>
#include <memory>
#include <string>
#include <map>
#include <filesystem>

namespace UnityAnalyzer::Unity::Objects {

struct SerializedObject {
    int64_t path_id;
    uint32_t class_id;
    std::string type_name;
    std::vector<uint8_t> data;
};

class ObjectReader {
public:
    virtual ~ObjectReader() = default;
    virtual bool Parse(const SerializedObject& obj, Core::ResourceEntry& entry) = 0;
    virtual bool Export(const Core::ResourceEntry& entry, const std::vector<uint8_t>& data, const std::filesystem::path& target_path) = 0;
};

class Texture2DReader : public ObjectReader {
public:
    bool Parse(const SerializedObject& obj, Core::ResourceEntry& entry) override;
    bool Export(const Core::ResourceEntry& entry, const std::vector<uint8_t>& data, const std::filesystem::path& target_path) override;
};

class MeshReader : public ObjectReader {
public:
    bool Parse(const SerializedObject& obj, Core::ResourceEntry& entry) override;
    bool Export(const Core::ResourceEntry& entry, const std::vector<uint8_t>& data, const std::filesystem::path& target_path) override;
};

class AudioClipReader : public ObjectReader {
public:
    bool Parse(const SerializedObject& obj, Core::ResourceEntry& entry) override;
    bool Export(const Core::ResourceEntry& entry, const std::vector<uint8_t>& data, const std::filesystem::path& target_path) override;
};

class TransformReader : public ObjectReader {
public:
    bool Parse(const SerializedObject& obj, Core::ResourceEntry& entry) override;
    bool Export(const Core::ResourceEntry& entry, const std::vector<uint8_t>& data, const std::filesystem::path& target_path) override;
};

class GameObjectReader : public ObjectReader {
public:
    bool Parse(const SerializedObject& obj, Core::ResourceEntry& entry) override;
    bool Export(const Core::ResourceEntry& entry, const std::vector<uint8_t>& data, const std::filesystem::path& target_path) override;
};

class MonoBehaviourReader : public ObjectReader {
public:
    bool Parse(const SerializedObject& obj, Core::ResourceEntry& entry) override;
    bool Export(const Core::ResourceEntry& entry, const std::vector<uint8_t>& data, const std::filesystem::path& target_path) override;
};

class TextAssetReader : public ObjectReader {
public:
    bool Parse(const SerializedObject& obj, Core::ResourceEntry& entry) override;
    bool Export(const Core::ResourceEntry& entry, const std::vector<uint8_t>& data, const std::filesystem::path& target_path) override;
};

class AnimationClipReader : public ObjectReader {
public:
    bool Parse(const SerializedObject& obj, Core::ResourceEntry& entry) override;
    bool Export(const Core::ResourceEntry& entry, const std::vector<uint8_t>& data, const std::filesystem::path& target_path) override;
};

class ShaderReader : public ObjectReader {
public:
    bool Parse(const SerializedObject& obj, Core::ResourceEntry& entry) override;
    bool Export(const Core::ResourceEntry& entry, const std::vector<uint8_t>& data, const std::filesystem::path& target_path) override;
};

class SpriteReader : public ObjectReader {
public:
    bool Parse(const SerializedObject& obj, Core::ResourceEntry& entry) override;
    bool Export(const Core::ResourceEntry& entry, const std::vector<uint8_t>& data, const std::filesystem::path& target_path) override;
};

class ObjectReaderFactory {
public:
    static std::unique_ptr<ObjectReader> CreateReader(const std::string& type_name);
    static std::unique_ptr<ObjectReader> CreateReaderByClassId(uint32_t class_id);
};

} // namespace UnityAnalyzer::Unity::Objects
