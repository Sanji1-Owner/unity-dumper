#pragma once
#include <cstdint>
#include "Il2CppGlobalMetadataHeader.h"

namespace UnityAnalyzer::IL2CPP::Structures {

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

struct Il2CppImageDefinition {
    StringIndex nameIndex;
    int32_t assemblyIndex;
    TypeDefinitionIndex typeStart;
    uint32_t typeCount;
    TypeDefinitionIndex exportedTypeStart;
    uint32_t exportedTypeCount;
    MethodIndex entryPointIndex;
    uint32_t token;
    CustomAttributeIndex customAttributeStart;
    uint32_t customAttributeCount;
};

struct Il2CppTypeDefinition {
    StringIndex nameIndex;
    StringIndex namespaceIndex;
    CustomAttributeIndex customAttributeIndex;
    TypeIndex byvalTypeIndex;
    TypeIndex byrefTypeIndex;
    TypeIndex declTypeIndex;
    TypeIndex parentIndex;
    TypeIndex elementTypeIndex;
    RGCTXIndex rgctxStartIndex;
    int32_t rgctxCount;
    GenericContainerIndex genericContainerIndex;
    uint32_t flags;
    FieldIndex fieldStart;
    MethodIndex methodStart;
    int32_t eventStart;
    int32_t propertyStart;
    int32_t nestedTypesStart;
    int32_t interfacesStart;
    int32_t vtableStart;
    int32_t interfaceOffsetsStart;
    uint16_t methodCount;
    uint16_t propertyCount;
    uint16_t fieldCount;
    uint16_t eventCount;
    uint16_t nestedTypeCount;
    uint16_t vtableCount;
    uint16_t interfacesCount;
    uint16_t interfaceOffsetsCount;
    uint32_t bitfield;
    uint32_t token;
};

struct Il2CppMethodDefinition {
    StringIndex nameIndex;
    TypeDefinitionIndex declaringType;
    TypeIndex returnType;
    ParameterIndex parameterStart;
    CustomAttributeIndex customAttributeIndex;
    GenericContainerIndex genericContainerIndex;
    MethodIndex methodIndex;
    MethodIndex invokerIndex;
    MethodIndex delegateWrapperIndex;
    RGCTXIndex rgctxStartIndex;
    int32_t rgctxCount;
    uint32_t token;
    uint16_t flags;
    uint16_t iflags;
    uint16_t slot;
    uint16_t parameterCount;
};

struct Il2CppParameterDefinition {
    StringIndex nameIndex;
    uint32_t token;
    CustomAttributeIndex customAttributeIndex;
    TypeIndex typeIndex;
};

struct Il2CppFieldDefinition {
    StringIndex nameIndex;
    TypeIndex typeIndex;
    CustomAttributeIndex customAttributeIndex;
    uint32_t token;
};

} // namespace UnityAnalyzer::IL2CPP::Structures
