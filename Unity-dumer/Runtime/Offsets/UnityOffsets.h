#pragma once

#include "OffsetProfile.h"
#include <vector>

namespace UnityAnalyzer::Runtime::Offsets {

// Define well-known offsets for common Unity versions
static const OffsetProfile UNITY_2019_4_LTS = {
    "2019.4.0f1",
    0, // Base address determined at runtime
    0x10, // GameObject -> m_Name offset (example)
    0x30, // GameObject -> m_Component offset
    0x08, // Component -> m_GameObject offset
    0x10, // Class -> name offset
    0x20  // Field -> offset_in_class offset
};

static const OffsetProfile UNITY_2020_3_LTS = {
    "2020.3.0f1",
    0,
    0x10,
    0x30,
    0x08,
    0x10,
    0x20
};

} // namespace UnityAnalyzer::Runtime::Offsets
