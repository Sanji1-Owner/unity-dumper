#include "Compression.h"
#include <lz4.h>
#include <lzma.h>
#include <iostream>
#include <cstring>

namespace UnityAnalyzer::Archives {

bool Compression::DecompressLZ4(const uint8_t* compressed_data, size_t compressed_size, uint8_t* uncompressed_data, size_t uncompressed_size) {
    if (!compressed_data || !uncompressed_data) return false;
    
    int result = LZ4_decompress_safe(
        reinterpret_cast<const char*>(compressed_data),
        reinterpret_cast<char*>(uncompressed_data),
        static_cast<int>(compressed_size),
        static_cast<int>(uncompressed_size)
    );
    
    return result >= 0; 
}

bool Compression::DecompressLZMA(const uint8_t* compressed_data, size_t compressed_size, uint8_t* uncompressed_data, size_t uncompressed_size) {
    if (!compressed_data || !uncompressed_data) return false;

    uint64_t memlimit = UINT64_MAX;
    size_t in_pos = 0;
    size_t out_pos = 0;

    lzma_ret ret = lzma_stream_buffer_decode(
        &memlimit,
        0,
        nullptr,
        compressed_data,
        &in_pos,
        compressed_size,
        uncompressed_data,
        &out_pos,
        uncompressed_size
    );

    if (ret != LZMA_OK) {
        std::cerr << "[Compression] LZMA decompression failed with error code: " << ret << std::endl;
        return false;
    }

    return out_pos == uncompressed_size;
}

} // namespace UnityAnalyzer::Archives
