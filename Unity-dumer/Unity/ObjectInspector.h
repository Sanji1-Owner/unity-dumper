#pragma once

#include "../Core/RuntimeContext.h"
#include "../Runtime/ClassInspector.h"
#include <string>
#include <vector>
#include <map>

namespace UnityAnalyzer::Unity {

struct UnityObjectEntry {
    int64_t instance_id;
    std::string type;
    std::string name;
    int64_t parent_id = 0;
    std::vector<int64_t> children_ids;
    std::map<std::string, std::string> metadata;
};

class ObjectInspector {
public:
    void Inspect(Core::RuntimeContext& context);
    
private:
    std::vector<UnityObjectEntry> ScanRuntimeObjects(Core::RuntimeContext& context);
};

} // namespace UnityAnalyzer::Unity
