#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <mutex>
#include <filesystem>

namespace UnityAnalyzer::Core {

enum class LogLevel {
    Debug,
    Info,
    Warning,
    Error
};

class Logger {
public:
    static Logger& Instance() {
        static Logger instance;
        return instance;
    }

    void Initialize(const std::filesystem::path& logPath) {
        std::lock_guard<std::mutex> lock(mutex_);
        if (logFile_.is_open()) logFile_.close();
        logFile_.open(logPath, std::ios::app);
    }

    void Log(LogLevel level, const std::string& message) {
        std::lock_guard<std::mutex> lock(mutex_);
        std::string levelStr = LevelToString(level);
        std::string formatted = "[" + levelStr + "] " + message;

        std::cout << formatted << std::endl;
        if (logFile_.is_open()) {
            logFile_ << formatted << std::endl;
        }
    }

private:
    Logger() = default;
    ~Logger() {
        if (logFile_.is_open()) logFile_.close();
    }

    std::string LevelToString(LogLevel level) {
        switch (level) {
            case LogLevel::Debug:   return "DEBUG";
            case LogLevel::Info:    return "INFO";
            case LogLevel::Warning: return "WARN";
            case LogLevel::Error:   return "ERROR";
            default:                return "UNKNOWN";
        }
    }

    std::mutex mutex_;
    std::ofstream logFile_;
};

#define LOG_DEBUG(msg) UnityAnalyzer::Core::Logger::Instance().Log(UnityAnalyzer::Core::LogLevel::Debug, msg)
#define LOG_INFO(msg)  UnityAnalyzer::Core::Logger::Instance().Log(UnityAnalyzer::Core::LogLevel::Info, msg)
#define LOG_WARN(msg)  UnityAnalyzer::Core::Logger::Instance().Log(UnityAnalyzer::Core::LogLevel::Warning, msg)
#define LOG_ERROR(msg) UnityAnalyzer::Core::Logger::Instance().Log(UnityAnalyzer::Core::LogLevel::Error, msg)

} // namespace UnityAnalyzer::Core
