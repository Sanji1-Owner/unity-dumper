#pragma once

#include "ArchiveReader.h"

namespace UnityAnalyzer::Archives {

class AddressableReader : public IArchiveReader {
public:
    bool Open(const std::filesystem::path& path) override {
        // Addressables usually have a specific catalog format or are bundles
        // Implementation would check for .json or .hash catalog files
        return path.extension() == ".json" && path.filename().string().find("catalog") != std::string::npos;
    }

    std::vector<std::string> ListFiles() override {
        return {};
    }

    bool Extract(const std::string& filename, const std::filesystem::path& destination) override {
        (void)filename; (void)destination;
        return false;
    }

    std::string GetType() const override { return "AddressableCatalog"; }
};

} // namespace UnityAnalyzer::Archives
