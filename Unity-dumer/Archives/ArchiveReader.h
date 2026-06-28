#pragma once

#include <string>
#include <vector>
#include <memory>
#include <filesystem>

namespace UnityAnalyzer::Archives {

class IArchiveReader {
public:
    virtual ~IArchiveReader() = default;
    virtual bool Open(const std::filesystem::path& path) = 0;
    virtual std::vector<std::string> ListFiles() = 0;
    virtual bool Extract(const std::string& filename, const std::filesystem::path& destination) = 0;
    virtual std::string GetType() const = 0;
};

class ArchiveRegistry {
public:
    static ArchiveRegistry& Instance() {
        static ArchiveRegistry instance;
        return instance;
    }

    void RegisterReader(std::unique_ptr<IArchiveReader> reader) {
        readers_.push_back(std::move(reader));
    }

    IArchiveReader* FindReader(const std::filesystem::path& path) {
        for (auto& reader : readers_) {
            if (reader->Open(path)) {
                return reader.get();
            }
        }
        return nullptr;
    }

private:
    ArchiveRegistry() = default;
    std::vector<std::unique_ptr<IArchiveReader>> readers_;
};

} // namespace UnityAnalyzer::Archives
