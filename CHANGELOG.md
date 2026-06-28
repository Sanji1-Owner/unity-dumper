# Changelog

All notable changes to this project will be documented in this file.

## [1.0.0-RC3] - 2026-XX-XX

### Added
- **IL2CPP Engine**: Version-aware metadata parsing (v16-v31) ported from industry-standard logic.
- **SDK Generation**: Automated C# source reconstruction with namespace support.
- **Asset Extractors**: Real Mesh (OBJ), Texture (DDS/TGA), and Audio (WAV) parsers.
- **Runtime Bridge**: Module discovery and symbol resolution for Windows and Android.
- **Diagnostics System**: Integrated stability audits and version support tracking.
- **Documentation**: Comprehensive Architecture, Build, and Usage guides.

### Changed
- Refactored project structure for better modularity.
- Standardized export pipeline to `DumpSession/` hierarchy.
- Hardened memory safety in binary parsers.

### Fixed
- Fixed audio metadata hardcoding.
- Fixed mesh index buffer parsing for 16/32-bit formats.
- Fixed IL2CPP version-specific header offsets.
- Resolved export path inconsistencies between platforms.
