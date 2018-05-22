cmake_minimum_required(VERSION 3.6)

project(sample)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall")

find_package (OpenGL REQUIRED)

set(SOURCE_FILES
    asset/AssetLoader.cpp
    gl3stub_wrapper.c
    image/jpgd.cpp
    image/ImageLoader.cpp
    image/RawImage.cpp
    image/Texture.cpp
    image/CubeTexture.cpp
    geometry/Mesh.cpp
    geometry/Box3.cpp
    scene/Object3D.cpp
    scene/Object3DComponent.cpp
    scene/Scene.cpp
    scene/Transform.cpp
    scene/Octree.cpp
    render/MeshRenderer.cpp
    render/Camera.cpp
    render/Renderer.cpp
    render/RendererES2.cpp
    render/ObjectRenderers.cpp
    material/Material.cpp
    material/BasicMaterial.cpp
    material/BasicTexturedMaterial.cpp
    material/BasicCubeMaterial.cpp
    material/Shader.cpp
    color/Color4Components.cpp
    math/Math.cpp
    math/Matrix4x4.cpp
    math/Quaternion.cpp
    util/Time.cpp
    Engine.cpp
    Demo.cpp)
    #RendererES3.cpp)

add_library(core ${SOURCE_FILES})

target_link_libraries(core ${OPENGL_LIBRARIES})
