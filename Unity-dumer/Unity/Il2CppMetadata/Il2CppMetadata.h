#pragma once

#include <cstdint>
#include <vector>
#include <string>

namespace UnityAnalyzer::Unity::Il2CppMetadata {

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

struct Il2CppGlobalMetadataHeader {
    uint32_t magic; // 0xFAB11BAF
    int32_t version;
    uint32_t stringLiteralOffset;
    int32_t stringLiteralCount;
    uint32_t stringLiteralDataOffset;
    int32_t stringLiteralDataCount;
    uint32_t stringOffset;
    int32_t stringCount;
    uint32_t eventsOffset;
    int32_t eventsCount;
    uint32_t propertiesOffset;
    int32_t propertiesCount;
    uint32_t methodsOffset;
    int32_t methodsCount;
    uint32_t parameterDefaultValuesOffset;
    int32_t parameterDefaultValuesCount;
    uint32_t fieldDefaultValuesOffset;
    int32_t fieldDefaultValuesCount;
    uint32_t fieldAndParameterDefaultValueDataOffset;
    int32_t fieldAndParameterDefaultValueDataCount;
    uint32_t fieldMarshaledSizesOffset;
    int32_t fieldMarshaledSizesCount;
    uint32_t parametersOffset;
    int32_t parametersCount;
    uint32_t fieldsOffset;
    int32_t fieldsCount;
    uint32_t genericParametersOffset;
    int32_t genericParametersCount;
    uint32_t genericParameterConstraintsOffset;
    int32_t genericParameterConstraintsCount;
    uint32_t genericContainersOffset;
    int32_t genericContainersCount;
    uint32_t nestedTypesOffset;
    int32_t nestedTypesCount;
    uint32_t interfacesOffset;
    int32_t interfacesCount;
    uint32_t vtableMethodsOffset;
    int32_t vtableMethodsCount;
    uint32_t interfaceOffsetsOffset;
    int32_t interfaceOffsetsCount;
    uint32_t typeDefinitionsOffset;
    int32_t typeDefinitionsCount;
    uint32_t rgctxEntriesOffset;
    int32_t rgctxEntriesCount;
    uint32_t imagesOffset;
    int32_t imagesCount;
    uint32_t assembliesOffset;
    int32_t assembliesCount;
    uint32_t metadataUsageListsOffset;
    int32_t metadataUsageListsCount;
    uint32_t metadataUsagePairsOffset;
    int32_t metadataUsagePairsCount;
    uint32_t fieldRefsOffset;
    int32_t fieldRefsCount;
    uint32_t referencedAssembliesOffset;
    int32_t referencedAssembliesCount;
    uint32_t attributesInfoOffset;
    int32_t attributesInfoCount;
    uint32_t attributeTypesOffset;
    int32_t attributeTypesCount;
};

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

} // namespace UnityAnalyzer::Unity::Il2CppMetadata
