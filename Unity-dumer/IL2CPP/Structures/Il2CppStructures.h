#pragma once
#include <cstdint>

namespace UnityAnalyzer::IL2CPP::Structures {

// Version independent definitions or version-specific wrappers
// For now, mirroring standard Unity definitions used in metadata v24+

typedef int32_t TypeIndex;
typedef int32_t TypeDefinitionIndex;
typedef int32_t FieldIndex;
typedef int32_t DefaultValueIndex;
typedef int32_t DefaultValueDataIndex;
typedef int32_t CustomAttributeIndex;
typedef int32_t ParameterIndex;
typedef int32_t MethodIndex;
typedef int32_t GenericMethodIndex;
typedef int32_t DecompressedMethodIndex;
typedef int32_t RGCTXIndex;
typedef int32_t StringIndex;
typedef int32_t StringLiteralIndex;
typedef int32_t GenericContainerIndex;
typedef int32_t GenericParameterIndex;
typedef int32_t GenericParameterConstraintIndex;

} // namespace UnityAnalyzer::IL2CPP::Structures
