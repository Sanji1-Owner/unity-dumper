#include "UnityFSParser.h"
#include "BlockReader.h"
#include "../Core/RuntimeContext.h"
#include <fstream>
#include <iostream>
#include <cstring>
#include <vector>
#include <algorithm>

namespace UnityAnalyzer::Archives {

bool UnityFSParser::Parse(const std::filesystem::path& path, Core::RuntimeContext& context) {
    std::ifstream file(path, std::ios::binary);
    if (!file.is_open()) return false;

    if (!ReadHeader(file)) return false;
    if (!ReadBlockInfo(file)) return false;

    auto index = context.GetResourceIndex();
    
    // Register the Bundle itself
    Core::ResourceEntry bundleEntry;
    bundleEntry.unique_id = "bundle_" + path.filename().string();
    bundleEntry.name = path.filename().string();
    bundleEntry.resource_type = Core::ResourceType::AssetBundle;
    bundleEntry.path = path.string();
    bundleEntry.metadata["unity_version"] = header_.unity_version;
    index->AddResource(bundleEntry);

    // Register each Node (Serialized File / Resource) inside the bundle
    for (const auto& node : nodes_) {
        Core::ResourceEntry nodeEntry;
        nodeEntry.unique_id = bundleEntry.unique_id + "/" + node.path;
        nodeEntry.name = node.path;
        nodeEntry.resource_type = Core::ResourceType::Unknown; 
        nodeEntry.parent = bundleEntry.unique_id;
        nodeEntry.metadata["offset"] = std::to_string(node.offset);
        nodeEntry.metadata["size"] = std::to_string(node.size);
        
        index->AddResource(nodeEntry);
        index->AddDependency(bundleEntry.unique_id, nodeEntry.unique_id);
    }

    return true;
}

bool UnityFSParser::ReadHeader(std::ifstream& file) {
    char sig[16];
    file.getline(sig, 16, '\0');
    header_.signature = sig;

    if (header_.signature != "UnityFS") return false;

    auto read_be32 = [](std::ifstream& f) {
        uint32_t val;
        f.read(reinterpret_cast<char*>(&val), 4);
        return (uint32_t)((val << 24) | ((val << 8) & 0x00FF0000) | ((val >> 8) & 0x0000FF00) | (val >> 24));
    };

    auto read_be64 = [](std::ifstream& f) {
        uint64_t val;
        f.read(reinterpret_cast<char*>(&val), 8);
        return (uint64_t)((val << 56) | ((val << 40) & 0x00FF000000000000) | ((val << 24) & 0x0000FF0000000000) | ((val << 8) & 0x000000FF00000000) |
                          ((val >> 8) & 0x00000000FF000000) | ((val >> 24) & 0x0000000000FF0000) | ((val >> 40) & 0x000000000000FF00) | (val >> 56));
    };

    header_.version = read_be32(file);
    
    char ver[32];
    file.getline(ver, 32, '\0');
    header_.unity_version = ver;
    
    file.getline(ver, 32, '\0');
    header_.unity_revision = ver;

    header_.size = read_be64(file);
    header_.compressed_block_info_size = read_be32(file);
    header_.uncompressed_block_info_size = read_be32(file);
    header_.flags = read_be32(file);

    return true;
}

bool UnityFSParser::ReadBlockInfo(std::ifstream& file) {
    // Determine info block location
    if ((header_.flags & 0x80) != 0) { // BlockInfo at end of file
        file.seekg(header_.size - header_.compressed_block_info_size);
    } else { // BlockInfo after header
        // Header size is dynamic due to strings
        // Current position is likely correct
    }

    std::vector<uint8_t> block_info_data = BlockReader::ReadBlock(file, 
        header_.compressed_block_info_size, 
        header_.uncompressed_block_info_size, 
        (uint16_t)header_.flags);

    if (block_info_data.empty()) return false;

    size_t offset = 0;
    auto read_be32 = [&]() {
        uint32_t val;
        std::memcpy(&val, &block_info_data[offset], 4);
        offset += 4;
        return (uint32_t)((val << 24) | ((val << 8) & 0x00FF0000) | ((val >> 8) & 0x0000FF00) | (val >> 24));
    };
    auto read_be64 = [&]() {
        uint64_t val;
        std::memcpy(&val, &block_info_data[offset], 8);
        offset += 8;
        return (uint64_t)((val << 56) | ((val << 40) & 0x00FF000000000000) | ((val << 24) & 0x0000FF0000000000) | ((val << 8) & 0x000000FF00000000) |
                          ((val >> 8) & 0x00000000FF000000) | ((val >> 24) & 0x0000000000FF0000) | ((val >> 40) & 0x000000000000FF00) | (val >> 56));
    };
    auto read_string = [&]() {
        std::string s;
        while (offset < block_info_data.size() && block_info_data[offset] != '\0') {
            s += (char)block_info_data[offset++];
        }
        offset++;
        return s;
    };

    // Hash (16 bytes)
    offset += 16;

    uint32_t block_count = read_be32();
    blocks_.clear();
    for (uint32_t i = 0; i < block_count; ++i) {
        StorageBlock block;
        block.uncompressed_size = read_be32();
        block.compressed_size = read_be32();
        block.flags = (uint16_t)read_be32();
        blocks_.push_back(block);
    }

    uint32_t node_count = read_be32();
    nodes_.clear();
    for (uint32_t i = 0; i < node_count; ++i) {
        Node node;
        node.offset = read_be64();
        node.size = read_be64();
        node.flags = read_be32();
        node.path = read_string();
        nodes_.push_back(node);
    }

    return true;
}

std::vector<uint8_t> UnityFSParser::GetNodeData(const Node& node, const std::filesystem::path& bundlePath) {
    std::ifstream file(bundlePath, std::ios::binary);
    if (!file.is_open()) return {};

    // Data starts after header and block info (unless flags say otherwise)
    // Actually, in UnityFS, data starts at a fixed offset or after block info.
    // For version 6+, it's often after the header strings.
    
    // We need to calculate the absolute stream offset for the uncompressed data.
    // Each block follows the previous one in the file.
    
    int64_t currentFileOffset = 0;
    // Header size + Block Info size
    // In ReadBlockInfo we seeked to get it. 
    // Usually header + info + blocks.
    
    // Simplification: calculate base offset of first block.
    // If info is at end, base is after header.
    // If info is after header, base is after info.
    
    // Let's assume standard layout.
    // Re-read header to get position after it.
    file.seekg(0);
    ReadHeader(file);
    if ((header_.flags & 0x80) == 0) { // Info after header
        file.seekg(header_.compressed_block_info_size, std::ios::cur);
    }
    int64_t dataBaseOffset = file.tellg();

    std::vector<uint8_t> result;
    result.reserve(static_cast<size_t>(node.size));

    int64_t uncompressedSeek = node.offset;
    int64_t remainingSize = node.size;

    int64_t blockUncompressedOffset = 0;
    int64_t blockFileOffset = dataBaseOffset;

    for (const auto& block : blocks_) {
        if (blockUncompressedOffset + block.uncompressed_size > uncompressedSeek) {
            // This block contains part of our node
            file.seekg(blockFileOffset);
            std::vector<uint8_t> uncompressedData = BlockReader::ReadBlock(file, block.compressed_size, block.uncompressed_size, block.flags);
            
            if (uncompressedData.empty()) return {};

            int64_t offsetInBlock = uncompressedSeek - blockUncompressedOffset;
            int64_t sizeToCopy = std::min(remainingSize, (int64_t)block.uncompressed_size - offsetInBlock);
            
            result.insert(result.end(), uncompressedData.begin() + offsetInBlock, uncompressedData.begin() + offsetInBlock + sizeToCopy);
            
            remainingSize -= sizeToCopy;
            uncompressedSeek += sizeToCopy;
            
            if (remainingSize <= 0) break;
        }
        blockUncompressedOffset += block.uncompressed_size;
        blockFileOffset += block.compressed_size;
    }

    return result;
}

} // namespace UnityAnalyzer::Archives
