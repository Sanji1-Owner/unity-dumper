#pragma once

#include <filesystem>
#include <string>

namespace UnityAnalyzer::Export {

struct ValidationResult {
    bool success;
    std::string message;
};

class ExportValidator {
public:
    static ValidationResult ValidateObj(const std::filesystem::path& path);
    static ValidationResult ValidateWav(const std::filesystem::path& path);
    static ValidationResult ValidateDds(const std::filesystem::path& path);
    static ValidationResult ValidateTga(const std::filesystem::path& path);
    static ValidationResult ValidateJson(const std::filesystem::path& path);
};

} // namespace UnityAnalyzer::Export
