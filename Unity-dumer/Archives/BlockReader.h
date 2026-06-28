#pragma once

#include <vector>
#include <cstdint>
#include <iostream>
#include "Compression.h"

namespace UnityAnalyzer::Archives {

class BlockReader {
public:
    static std::vector<uint8_t> ReadBlock(std::istream& stream, uint32_t compressed_size, uint32_t uncompressed_size, uint16_t flags);

private:
    static CompressionType GetCompressionType(uint16_t flags);
};

} // namespace UnityAnalyzer::Archives
