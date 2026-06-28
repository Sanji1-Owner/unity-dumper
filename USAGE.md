# Usage Guide

UnityAnalyzer is designed to work with both static files and live runtime processes.

## Input Requirements

To perform a full analysis and dump, the following files are typically required:

- **Unity Bundles / Assets**: `.unity3d`, `.assets`, etc., for asset extraction.
- **Global Metadata**: `global-metadata.dat` (found in `Data/Managed/Metadata/`).
- **IL2CPP Binary**: 
  - Windows: `GameAssembly.dll`
  - Android: `libil2cpp.so`

## Running UnityAnalyzer

### Windows (Desktop)
Run the executable via command line:
```bash
UnityAnalyzer.exe --path "C:/Path/To/Game" --metadata "C:/Path/To/global-metadata.dat"
```

### Android (Runtime)
The library `libUnityAnalyzer.so` can be injected or loaded by a bridge to analyze the process memory in real-time.

## Configuration
Before running, you can modify `config.json` to set your desired output path:
```json
{
  "output_path": "./DumpSession"
}
```

## Understanding the Output

Once the analysis is complete, results are saved in the `DumpSession` folder:

- **Assets/**: Contains reconstructed `.obj` meshes, `.wav` audio clips, and `.dds`/`.tga` textures.
- **Metadata/**: Contains JSON databases of the IL2CPP environment.
- **SDK/**: Contains a full C# project structure with source files for every assembly.
- **Database/**: Contains mappings between IL2CPP tokens and runtime addresses.
- **Diagnostics/**: Contains build and stability reports.
