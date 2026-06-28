#include "BlockReader.h"
#include <vector>

namespace UnityAnalyzer::Archives {

std::vector<uint8_t> BlockReader::ReadBlock(std::istream& stream, uint32_t compressed_size, uint32_t uncompressed_size, uint16_t flags) {
    std::vector<uint8_t> compressed_data(compressed_size);
    stream.read(reinterpret_cast<char*>(compressed_data.data()), compressed_size);

    CompressionType compression = GetCompressionType(flags);
    if (compression == CompressionType::None) {
        return compressed_data;
    }

    std::vector<uint8_t> uncompressed_data(uncompressed_size);
    bool success = false;

    switch (compression) {
        case CompressionType::LZ4:
        case CompressionType::LZ4HC:
            success = Compression::DecompressLZ4(compressed_data.data(), compressed_size, uncompressed_data.data(), uncompressed_size);
            break;
        case CompressionType::LZMA:
            success = Compression::DecompressLZMA(compressed_data.data(), compressed_size, uncompressed_data.data(), uncompressed_size);
            break;
        default:
            return {};
    }

    return success ? uncompressed_data : std::vector<uint8_t>();
}

CompressionType BlockReader::GetCompressionType(uint16_t flags) {
    return static_cast<CompressionType>(flags & 0x3F);
}

} // namespace UnityAnalyzer::Archives
