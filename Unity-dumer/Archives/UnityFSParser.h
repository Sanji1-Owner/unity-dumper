#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <filesystem>
#include <memory>
#include "Compression.h"

namespace UnityAnalyzer::Core { class RuntimeContext; }

namespace UnityAnalyzer::Archives {

struct UnityFSHeader {
    std::string signature;
    uint32_t version;
    std::string unity_version;
    std::string unity_revision;
    int64_t size;
    uint32_t compressed_block_info_size;
    uint32_t uncompressed_block_info_size;
    uint32_t flags;
};

struct StorageBlock {
    uint32_t uncompressed_size;
    uint32_t compressed_size;
    uint16_t flags;
};

struct Node {
    int64_t offset;
    int64_t size;
    uint32_t flags;
    std::string path;
};

class UnityFSParser {
public:
    bool Parse(const std::filesystem::path& path, Core::RuntimeContext& context);
    std::vector<uint8_t> GetNodeData(const Node& node, const std::filesystem::path& bundlePath);

    const UnityFSHeader& GetHeader() const { return header_; }
    const std::vector<Node>& GetNodes() const { return nodes_; }

private:
    UnityFSHeader header_;
    std::vector<StorageBlock> blocks_;
    std::vector<Node> nodes_;

    bool ReadHeader(std::ifstream& file);
    bool ReadBlockInfo(std::ifstream& file);
};

} // namespace UnityAnalyzer::Archives
