#pragma once

#include "ResourceEntry.h"
#include <unordered_map>
#include <mutex>
#include <memory>

namespace UnityAnalyzer::Core {

class ResourceIndex {
public:
    void AddResource(const ResourceEntry& entry);
    void UpdateResource(const ResourceEntry& entry);
    std::shared_ptr<ResourceEntry> GetResource(const std::string& id);
    std::vector<std::shared_ptr<ResourceEntry>> GetResourcesByType(ResourceType type);
    
    nlohmann::json ExportResources() const;
    nlohmann::json ExportDependencies() const;

private:
    std::unordered_map<std::string, std::shared_ptr<ResourceEntry>> resources_;
    mutable std::mutex mutex_;
};

} // namespace UnityAnalyzer::Core
