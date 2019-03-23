# Core
Light-weight, C/C++ based, 3D rendering library built on OpenGL.

## Current features:

* Multi-pass and single-pass forward rendering
* Model import: Support for many industry standard formats via the Assimp library
* Image import: Support for many industry standard formats via the DevIL library
* Dynamic shadows via shadow maps
  - Support for directional and point lights (2D and cube shadow maps)
  - Multiple cascade levels for directional lights
  - Soft edges through PCF
* Physically-based rendering
  - Cook-Torrance BRDF
  - Image-based ambient lighting for global illumination approximation
  - HDR support
  - Gamma correction support

## External dependencies:

- Assimp (Asset Import Library)
- DevIL image library

Dynamic versions of the above libraries are required.

## Building

Standard CMake build is currently supported. You will need to modify the locations of the Assimp and DevIL libraries in CMakeLists.txt. 
