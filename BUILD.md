# Build Guide

This document provides instructions for building UnityAnalyzer from source on various platforms.

## Prerequisites
- CMake (3.10 or higher)
- C++17 compliant compiler (MSVC 2019+, GCC 9+, or Clang 10+)

---

## Windows (Desktop)

### Using CMake CLI
1. Open a terminal or Developer Command Prompt for VS.
2. Navigate to the project root.
3. Run the following commands:
   ```bash
   mkdir build
   cd build
   cmake ..
   cmake --build . --config Release
   ```

### Using Visual Studio
1. Open the project folder in Visual Studio.
2. Visual Studio will automatically detect the `CMakeLists.txt` and configure the project.
3. Select `UnityAnalyzer.exe` as the startup item and build.

---

## Linux (Desktop)

### Install Dependencies
```bash
sudo apt-get update
sudo apt-get install build-essential cmake liblzma-dev
```

### Build Commands
```bash
mkdir build
cd build
cmake ..
make -j$(nproc)
```

---

## Android (Library)

UnityAnalyzer can be built as a shared library (`.so`) for integration into Android applications or runtime bridges.

### Prerequisites
- Android NDK (r21 or higher)

### Build Commands
```bash
mkdir build_android
cd build_android
cmake -DCMAKE_TOOLCHAIN_FILE=$ANDROID_NDK/build/cmake/android.toolchain.cmake \
      -DANDROID_ABI=arm64-v8a \
      -DANDROID_PLATFORM=android-24 \
      ..
make -j$(nproc)
```

The resulting `libUnityAnalyzer.so` will be located in the build directory.
