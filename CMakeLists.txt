cmake_minimum_required(VERSION 3.6)

project(Core)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall")

find_package (OpenGL REQUIRED)

set(SOURE_DIR ${CMAKE_SOURCE_DIR})


set(INCLUDE_DIRS
    asset
    image
    geometry
    scene
    render
    material
    color
    math
    util)

set(INCLUDE_FILES
    asset/AssetLoader.h
    image/jpgd.h
    image/ImageLoader.h
    image/RawImage.h
    image/Texture.h
    image/CubeTexture.h
    geometry/Mesh.h
    geometry/Box3.h
    scene/Object3D.h
    scene/Object3DComponent.h
    scene/Scene.h
    scene/Transform.h
    scene/Octree.h
    render/MeshRenderer.h
    render/Camera.h
    render/Renderer.h
    render/RendererES2.h
    render/ObjectRenderers.h
    material/Material.h
    material/BasicMaterial.h
    material/BasicTexturedMaterial.h
    material/BasicCubeMaterial.h
    material/Shader.h
    color/Color4Components.h
    math/Math.h
    math/Matrix4x4.h
    math/Quaternion.h
    util/Time.h
    Engine.h
    Demo.h)
    #RendererES3.h)

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

add_custom_target(MakeIncludeDir ALL COMMAND ${CMAKE_COMMAND} -E make_directory "include")

# do the copying
foreach( dir_i ${INCLUDE_DIRS})
    add_custom_command(
    TARGET core
    POST_BUILD
    COMMAND ${CMAKE_COMMAND}
    ARGS -E make_directory "include/${dir_i}"
)
endforeach( dir_i )

# do the copying
foreach( file_i ${INCLUDE_FILES})
    add_custom_command(
    TARGET core
    POST_BUILD
    COMMENT "copying ${CMAKE_SOURCE_DIR}/${file_i} to  include/${file_i}"
    COMMAND ${CMAKE_COMMAND}
    ARGS -E copy "${CMAKE_SOURCE_DIR}/${file_i}" "include/${file_i}"
)
endforeach( file_i )

# file(COPY ${INCLUDE_FILES} DESTINATION "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/include")
