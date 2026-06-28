#include "ScriptAnalyzer.h"
#include "../Core/ResourceEntry.h"
#include <filesystem>
#include <algorithm>

namespace UnityAnalyzer::Scripts {

void ScriptAnalyzer::Analyze(Core::RuntimeContext& context) {
    ScanAssemblies(context);
    ScanLua(context);

    auto index = context.GetResourceIndex();
    for (const auto& s : scripts_) {
        Core::ResourceEntry entry;
        entry.unique_id = "script_" + s.id;
        entry.name = s.name;
        entry.path = s.path;
        entry.resource_type = Core::ResourceType::Script;
        entry.metadata = s.metadata;
        entry.metadata["language"] = s.language;
        
        index->AddResource(entry);
    }
}

void ScriptAnalyzer::ScanAssemblies(Core::RuntimeContext& context) {
    // In a real implementation, we would use a library like Cecil or internal Unity APIs
    // to reflect on the loaded assemblies.
    
    ScriptEntry mainScript;
    mainScript.id = "asm_csharp";
    mainScript.name = "Assembly-CSharp.dll";
    mainScript.language = "C#";
    mainScript.metadata["type"] = "Assembly";
    
    scripts_.push_back(mainScript);
}

void ScriptAnalyzer::ScanLua(Core::RuntimeContext& context) {
    // Detect Lua engines and their script caches.
    // Xlua, Tolua, and uLua are common.
    
    // Simulate finding a Lua script
    ScriptEntry luaScript;
    luaScript.id = "lua_init";
    luaScript.name = "GameInit.lua";
    luaScript.language = "Lua";
    luaScript.path = "Assets/Scripts/Lua/GameInit.lua.txt";
    
    scripts_.push_back(luaScript);
}

} // namespace UnityAnalyzer::Scripts
