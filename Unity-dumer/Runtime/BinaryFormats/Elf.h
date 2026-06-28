#pragma once

#include <string>
#include <vector>
#include <cstdint>

namespace UnityAnalyzer::Runtime::BinaryFormats {

struct ElfSection {
    std::string name;
    uintptr_t virtualAddress;
    uint64_t size;
};

class ElfParser {
public:
    bool Load(const std::string& path);
    const std::vector<ElfSection>& GetSections() const { return sections_; }

private:
    std::vector<ElfSection> sections_;
};

} // namespace UnityAnalyzer::Runtime::BinaryFormats
