# Core
Light-weight, C/C++ based, 3D rendering library built on OpenGL. Ultimately intended to be the successor to/replacement for my older gaming engine, GTE.

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
* Skeletal animation:
  - Vertex skinning
  - Animation blending
* Scene-graph API
* Render-to-texture capability
* Screen-space ambient occlusion

## External dependencies:

- Assimp (Asset Import Library)
- DevIL image library

Dynamic versions of the above libraries are required.

## Building
Standard CMake build is currently supported. 

#### External Libraries
You will need to modify the locations of the Assimp and DevIL libraries in CMakeLists.txt. The relevant variables are DEVIL_DIR, ASSIMP_INCLUDE_DIR, and ASSIMP_BUILD_DIR. 

**Important:** Currently only version 3.1.1 of the Asset Import Library works 100% correctly with animations, so you'll want to make sure you either build or install that version if the animations feature is necessary for you.

#### OpenGL & Linux
In order for CMake to find your OpenGL installation on some Linux distributions, it may be necessary to run the following command to install the Mesa development packages:

     sudo apt-get install libgl1-mesa-dev


