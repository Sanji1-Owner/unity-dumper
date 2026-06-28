# UnityAnalyzer

UnityAnalyzer is a high-performance, modular C++ framework designed for the analysis, extraction, and reconstruction of Unity engine assets and IL2CPP metadata.

**Owner**: Sanji1-Owner

---

## Project Description

UnityAnalyzer provides a comprehensive suite of tools for reverse engineering Unity-based applications. It handles the entire pipeline from raw archive parsing to high-level C# SDK generation and runtime memory analysis.

### Core Capabilities:
- **UnityFS Parsing**: Robust handling of Unity bundle files with integrated LZ4 and LZMA decompression.
- **SerializedFile Analysis**: Deep parsing of Unity's serialized format, including recursive TypeTree reconstruction.
- **IL2CPP Metadata Analysis**: Version-aware parsing of `global-metadata.dat` (supporting v16 through v31).
- **SDK Generation**: Automated reconstruction of C# source code from IL2CPP metadata, organized by assembly and namespace.
- **Export Pipeline**: Standardized output structure for assets, metadata, and runtime diagnostics.

---

## Architecture

The project is organized into modular layers:

- **Archives**: Low-level parsing for UnityFS, SerializedFiles, and compression (LZ4/LZMA).
- **Unity Objects**: Specialized readers for core Unity types (Mesh, Texture2D, AudioClip, GameObject, etc.).
- **IL2CPP**: The core engine for metadata parsing, version handling, and SDK generation.
- **Runtime**: Bridging the gap between static files and live memory, providing runtime address resolution.
- **Export**: Managing the `DumpSession` filesystem structure and post-export validation.
- **Diagnostics**: Real-time logging, stability audits, and version support matrices.

---

## Features

### Verified (Stable)
- **Archive Support**: UnityFS, SerializedFile, TypeTree.
- **Compression**: Real LZ4 and LZMA (liblzma) integration.
- **Asset Extraction**:
  - **Textures**: Export to DDS, TGA, and Raw formats.
  - **Meshes**: Verified OBJ export with vertex and index buffer support.
  - **Audio**: Verified WAV reconstruction with proper frequency/channel sync.
  - **Hierarchy**: Full GameObject/Transform parent-child reconstruction.
- **IL2CPP Engine**: Metadata parsing (v16-v31) and full SDK C# source generation.

### Partial / Experimental
- **Animation**: Metadata and name extraction (Curve data in development).
- **Shader**: Raw bytecode extraction (Decompiler planned for future release).
- **Dummy DLL**: Organized C# source assemblies (Binary PE generation partial).

---

## Output Structure

All extracted data is organized into a `DumpSession/` directory:

```text
DumpSession/
├── Assets/        # Extracted Mesh, Texture, and Audio files
├── Metadata/      # IL2CPP json databases (assemblies, classes, etc.)
├── SDK/           # Reconstructed C# source code (.cs)
├── Database/      # Relational links and runtime address mappings
└── Diagnostics/   # Build reports and stability audits
```

---

## License

This project is licensed under the terms found in `LICENSE.md`.
Copyright (C) 2026 Sanji1-Owner.
"Original project by Sanji1-Owner" must be maintained in all derivatives.
