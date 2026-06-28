#pragma once

#include <string>
#include "../Core/RuntimeContext.h"

namespace UnityAnalyzer::Plugins {

class IUnityModule {
public:
    virtual ~IUnityModule() = default;

    virtual uint32_t AbiVersion() const = 0;
    virtual std::string Name() const = 0;
    
    virtual void Initialize(Core::RuntimeContext& context) = 0;
    virtual bool Detect() = 0;
    virtual void Scan() = 0;
    virtual void Export() = 0;
};

} // namespace UnityAnalyzer::Plugins
