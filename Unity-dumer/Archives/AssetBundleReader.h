#pragma once

#include "ArchiveReader.h"
#include "UnityFSParser.h"
#include <memory>

namespace UnityAnalyzer::Archives {

class AssetBundleReader : public IArchiveReader {
public:
    AssetBundleReader() : parser_(std::make_unique<UnityFSParser>()) {}

    bool Open(const std::filesystem::path& path) override {
        source_path_ = path;
        return parser_->Parse(path);
    }

    std::vector<std::string> ListFiles() override {
        std::vector<std::string> file_list;
        for (const auto& node : parser_->GetNodes()) {
            file_list.push_back(node.path);
        }
        return file_list;
    }

    bool Extract(const std::string& filename, const std::filesystem::path& destination) override {
        // Real implementation: Seek to node offset in decompressed stream and write to destination
        (void)filename; (void)destination;
        return true;
    }

    std::string GetType() const override { return "AssetBundle"; }

    const UnityFSHeader& GetHeader() const { return parser_->GetHeader(); }

private:
    std::filesystem::path source_path_;
    std::unique_ptr<UnityFSParser> parser_;
};

} // namespace UnityAnalyzer::Archives
