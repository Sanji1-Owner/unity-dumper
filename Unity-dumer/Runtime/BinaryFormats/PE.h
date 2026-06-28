#pragma once

#include <string>
#include <vector>
#include <cstdint>

namespace UnityAnalyzer::Runtime::BinaryFormats {

struct PESection {
    std::string name;
    uint32_t virtualAddress;
    uint32_t virtualSize;
    uint32_t rawDataOffset;
    uint32_t rawDataSize;
};

class PEParser {
public:
    bool Load(const std::string& path);
    uintptr_t GetImageBase() const { return imageBase_; }
    const std::vector<PESection>& GetSections() const { return sections_; }
    uint32_t RvaToOffset(uint32_t rva) const;

private:
    uintptr_t imageBase_ = 0;
    std::vector<PESection> sections_;
};

} // namespace UnityAnalyzer::Runtime::BinaryFormats
