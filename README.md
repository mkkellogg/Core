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
* Particle systems
* Scene-graph API
* Render-to-texture capability
* Screen-space ambient occlusion

## External dependencies:

- Assimp (Asset Import Library)
- DevIL image library

Dynamic versions of the above libraries are required.

## Building
Standard CMake build is currently supported and an out-of-source build is recommended. To generate the make files, execute the command relevant to the desired configuration:

Debug:

    cmake -G "Unix Makefiles"

Release:

    cmake -DCMAKE_BUILD_TYPE=Release -G "Unix Makefiles"
    
A release build will be significantly more performant.

#### External Libraries
You will need to modify the locations of the Assimp and DevIL libraries in CMakeLists.txt. The relevant variables are DEVIL_DIR and ASSIMP_DIR.

#### Linux notes
In order for CMake to find your OpenGL installation on some Linux distributions, it may be necessary to run the following command to install the Mesa development packages:

     sudo apt-get install libgl1-mesa-dev

The DevIL library can be installed via:

     sudo apt-get install libdevil-dev

