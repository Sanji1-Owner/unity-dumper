#pragma once

#include "../Core/RuntimeContext.h"
#include "ObjectInspector.h"
#include <vector>

namespace UnityAnalyzer::Unity {

class UnityScanner {
public:
    UnityScanner(Core::RuntimeContext& context);
    
    void ScanStaticObjects();
    void ScanDynamicObjects();

private:
    Core::RuntimeContext& context_;
    TextureInspector texture_inspector_;
    MeshInspector mesh_inspector_;
};

} // namespace UnityAnalyzer::Unity
