# UnityAnalyzer Architecture

UnityAnalyzer is a modular framework for analyzing and dumping Unity game data, specifically focusing on IL2CPP-based games.

## Core Modules

### 1. Archives
Handles various Unity archive formats, including UnityFS, SerializedFile, and compression (LZ4/LZMA).
- **UnityFSParser**: Parses Unity bundle files.
- **SerializedFile**: Parses Unity serialized files and reconstructions TypeTrees.
- **Compression**: Provides LZ4 and LZMA decompression services.

### 2. Unity Objects
Contains specialized readers for Unity Class IDs.
- **ObjectReader**: Base class for all object parsers.
- **Texture2DReader**: Extracts textures to DDS/TGA.
- **MeshReader**: Extracts geometry to OBJ.
- **AudioClipReader**: Reconstructs WAV files.
- **GameObject/Transform**: Reconstructs scene hierarchy.

### 3. IL2CPP
The core engine for metadata and binary analysis.
- **MetadataReader**: Parses `global-metadata.dat`.
- **SDKExporter**: Generates C# source code from metadata.
- **MetadataAnalyzer**: Performs cross-referencing between types and images.

### 4. Runtime
Handles live process analysis and memory reading.
- **MemoryReader**: Abstracted memory access for Windows (GameAssembly.dll) and Android (libil2cpp.so).
- **Il2CppRuntimeBridge**: Resolves runtime addresses and method pointers.
- **OffsetManager**: Manages version-specific offsets for runtime structures.

### 5. Export
Final stage of the pipeline.
- **DumpExporter**: Manages the `DumpSession` directory and exports processed data.
- **ExportValidator**: Ensures exported files (OBJ, WAV, JSON) are valid.

### 6. Diagnostics
System health and audit tools.
- **Logger**: Centralized logging.
- **Audit Tools**: Generates reports on code status and feature support.

## Data Flow

1. **Input**: Archives or Live Process.
2. **Parser**: Binary parsing into internal structures.
3. **Analyzer**: Connecting metadata to binary or resources.
4. **Exporter**: Writing validated files to `DumpSession/`.
