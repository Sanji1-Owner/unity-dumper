#include "ExportValidator.h"
#include <fstream>
#include <vector>
#include <nlohmann/json.hpp>

namespace UnityAnalyzer::Export {

ValidationResult ExportValidator::ValidateObj(const std::filesystem::path& path) {
    std::ifstream file(path);
    if (!file.is_open()) return {false, "Could not open file"};

    std::string line;
    bool has_vertices = false;
    bool has_faces = false;
    while (std::getline(file, line)) {
        if (line.substr(0, 2) == "v ") has_vertices = true;
        if (line.substr(0, 2) == "f ") has_faces = true;
        if (has_vertices && has_faces) break;
    }

    if (!has_vertices) return {false, "Missing vertices"};
    if (!has_faces) return {false, "Missing faces"};
    return {true, "Valid OBJ"};
}

ValidationResult ExportValidator::ValidateWav(const std::filesystem::path& path) {
    std::ifstream file(path, std::ios::binary);
    if (!file.is_open()) return {false, "Could not open file"};

    char riff[4];
    file.read(riff, 4);
    if (std::string(riff, 4) != "RIFF") return {false, "Missing RIFF signature"};

    file.seekg(8);
    char wave[4];
    file.read(wave, 4);
    if (std::string(wave, 4) != "WAVE") return {false, "Missing WAVE signature"};

    return {true, "Valid WAV"};
}

ValidationResult ExportValidator::ValidateDds(const std::filesystem::path& path) {
    std::ifstream file(path, std::ios::binary);
    if (!file.is_open()) return {false, "Could not open file"};

    char magic[4];
    file.read(magic, 4);
    if (std::string(magic, 4) != "DDS ") return {false, "Missing DDS signature"};

    return {true, "Valid DDS"};
}

ValidationResult ExportValidator::ValidateTga(const std::filesystem::path& path) {
    std::ifstream file(path, std::ios::binary);
    if (!file.is_open()) return {false, "Could not open file"};

    file.seekg(0, std::ios::end);
    size_t size = file.tellg();
    if (size < 18) return {false, "TGA too small"};

    return {true, "Valid TGA (Basic Check)"};
}

ValidationResult ExportValidator::ValidateJson(const std::filesystem::path& path) {
    std::ifstream file(path);
    if (!file.is_open()) return {false, "Could not open file"};

    try {
        nlohmann::json j;
        file >> j;
        return {true, "Valid JSON"};
    } catch (const std::exception& e) {
        return {false, std::string("JSON Parse Error: ") + e.what()};
    }
}

} // namespace UnityAnalyzer::Export
