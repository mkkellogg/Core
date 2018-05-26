cmake_minimum_required(VERSION 3.6)

project(Core)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall")

find_package (OpenGL REQUIRED)

set(INCLUDE_DIRS
    asset
    image
    geometry
    scene
    render
    material
    color
    math
    util
    common)

set(INCLUDE_FILES
    geometry/Box3.h
    geometry/Vector2Components.h
    geometry/Mesh.h
    geometry/Vector3Components.h
    geometry/Vector3.h
    geometry/Vector2.h
    geometry/AttributeArray.h
    scene/Object3D.h
    scene/Scene.h
    scene/Object3DComponent.h
    scene/Transform.h
    scene/Octree.h
    Engine.h
    asset/AssetLoader.h
    material/BasicMaterial.h
    material/Material.h
    material/StandardUniforms.h
    material/BasicTexturedMaterial.h
    material/StandardAttributes.h
    material/BasicCubeMaterial.h
    material/Shader.h
    Demo.h
    render/BaseRenderableContainer.h
    render/RenderableContainer.h
    render/Renderable.h
    render/ObjectRenderers.h
    render/BaseObjectRenderer.h
    render/ObjectRenderer.h
    render/Camera.h
    render/RendererGL.h
    render/Renderer.h
    render/BaseRenderable.h
    render/MeshRenderer.h
    image/ImageLoader.h
    image/CubeTexture.h
    image/Texture.h
    image/jpgd.h
    image/RawImage.h
    color/Color.h
    color/Color4Components.h
    util/Time.h
    math/Math.h
    math/Quaternion.h
    math/Matrix4x4.h
    base/VectorStorage.h
    base/BaseVector.h
    common/gl.h
    common/assert.h
    common/types.h
    common/debug.h)

set(SOURCE_FILES
    common/Debug.cpp
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
    render/BaseRenderableContainer.cpp
    render/RenderableContainer.cpp
    render/MeshRenderer.cpp
    render/Camera.cpp
    render/Renderer.cpp
    render/RendererGL.cpp
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
add_custom_target(MakeIncludeCoreDir ALL COMMAND ${CMAKE_COMMAND} -E make_directory "include/Core")

# do the copying
foreach(dir_i ${INCLUDE_DIRS})
    add_custom_command(
    TARGET core
    POST_BUILD
    COMMAND ${CMAKE_COMMAND}
    ARGS -E make_directory "include/Core/${dir_i}"
)
endforeach(dir_i)

# do the copying
foreach(file_i ${INCLUDE_FILES})
    add_custom_command(
    TARGET core
    POST_BUILD
    # COMMENT "copying ${CMAKE_SOURCE_DIR}/${file_i} to  include/Core/${file_i}"
    COMMAND ${CMAKE_COMMAND}
    ARGS -E copy "${CMAKE_SOURCE_DIR}/${file_i}" "include/Core/${file_i}"
)
endforeach(file_i)
