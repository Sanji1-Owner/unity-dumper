#pragma once

#include <functional>
#include <map>
#include <vector>
#include <string>
#include <any>
#include <mutex>

namespace UnityAnalyzer::Core {

enum class EventType {
    DetectionStarted,
    DetectionFinished,
    ResourceIndexed,
    AnalysisStep,
    ExportStarted,
    ExportFinished,
    Error
};

class EventSystem {
public:
    using EventHandler = std::function<void(EventType, const std::any&)>;

    static EventSystem& Instance() {
        static EventSystem instance;
        return instance;
    }

    void Subscribe(EventType type, const EventHandler& handler) {
        std::lock_guard<std::mutex> lock(mutex_);
        handlers_[type].push_back(handler);
    }

    void Publish(EventType type, const std::any& data = {}) {
        std::lock_guard<std::mutex> lock(mutex_);
        if (handlers_.find(type) != handlers_.end()) {
            for (auto& handler : handlers_[type]) {
                handler(type, data);
            }
        }
    }

private:
    EventSystem() = default;
    std::map<EventType, std::vector<EventHandler>> handlers_;
    std::mutex mutex_;
};

} // namespace UnityAnalyzer::Core
