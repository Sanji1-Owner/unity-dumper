#include "ResourceIndex.h"

namespace UnityAnalyzer::Core {

void ResourceIndex::AddResource(const ResourceEntry& entry) {
    std::lock_guard<std::mutex> lock(mutex_);
    resources_[entry.unique_id] = std::make_shared<ResourceEntry>(entry);
}

void ResourceIndex::UpdateResource(const ResourceEntry& entry) {
    std::lock_guard<std::mutex> lock(mutex_);
    resources_[entry.unique_id] = std::make_shared<ResourceEntry>(entry);
}

std::shared_ptr<ResourceEntry> ResourceIndex::GetResource(const std::string& id) {
    std::lock_guard<std::mutex> lock(mutex_);
    auto it = resources_.find(id);
    if (it != resources_.end()) {
        return it->second;
    }
    return nullptr;
}

std::vector<std::shared_ptr<ResourceEntry>> ResourceIndex::GetResourcesByType(ResourceType type) {
    std::lock_guard<std::mutex> lock(mutex_);
    std::vector<std::shared_ptr<ResourceEntry>> result;
    for (const auto& [id, entry] : resources_) {
        if (entry->resource_type == type) {
            result.push_back(entry);
        }
    }
    return result;
}

nlohmann::json ResourceIndex::ExportResources() const {
    std::lock_guard<std::mutex> lock(mutex_);
    nlohmann::json j = nlohmann::json::array();
    for (const auto& [id, entry] : resources_) {
        j.push_back(entry->to_json());
    }
    return j;
}

nlohmann::json ResourceIndex::ExportDependencies() const {
    std::lock_guard<std::mutex> lock(mutex_);
    nlohmann::json j = nlohmann::json::object();
    for (const auto& [id, entry] : resources_) {
        if (!entry->dependencies.empty()) {
            j[id] = entry->dependencies;
        }
    }
    return j;
}

} // namespace UnityAnalyzer::Core
