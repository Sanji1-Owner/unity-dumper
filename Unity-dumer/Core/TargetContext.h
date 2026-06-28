#pragma once

#include <string>
#include <variant>
#include <filesystem>

namespace UnityAnalyzer::Core {

enum class TargetType {
    LocalFile,
    ProcessMemory,
    AndroidProcess,
    RemoteDump
};

struct TargetContext {
    TargetType type;
    std::variant<std::filesystem::path, uint32_t, std::string> target;
    std::string platform;

    static TargetContext CreateLocal(const std::filesystem::path& path) {
        return { TargetType::LocalFile, path, "" };
    }

    static TargetContext CreateProcess(uint32_t pid) {
        return { TargetType::ProcessMemory, pid, "" };
    }
};

} // namespace UnityAnalyzer::Core
