#pragma once

#include <vector>
#include <cstdint>

namespace UnityAnalyzer::Archives {

enum class CompressionType {
    None = 0,
    LZMA = 1,
    LZ4 = 2,
    LZ4HC = 3,
    LZH = 4
};

class Compression {
public:
    static bool DecompressLZ4(const uint8_t* compressed_data, size_t compressed_size, uint8_t* uncompressed_data, size_t uncompressed_size);
    static bool DecompressLZMA(const uint8_t* compressed_data, size_t compressed_size, uint8_t* uncompressed_data, size_t uncompressed_size);
};

} // namespace UnityAnalyzer::Archives
