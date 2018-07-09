cmake_minimum_required(VERSION 3.6)

project(Core)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -fPIC")

find_package (OpenGL REQUIRED)

set(EXECUTABLE_NAME core)

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
    common
    GL)

set(INCLUDE_FILES
    filesys/FileSystem.h
    filesys/FileSystemIX.h
    geometry/Box3.h
    geometry/Vector2Components.h
    geometry/Vector2.h
    geometry/Mesh.h
    geometry/Vector3Components.h
    geometry/Vector3.h
    geometry/Vector4Components.h
    geometry/Vector4.h
    geometry/AttributeArray.h
    geometry/AttributeType.h
    geometry/AttributeArrayGPUStorage.h
    geometry/IndexBuffer.h
    geometry/GeometryUtils.h
    scene/Object3D.h
    scene/Scene.h
    scene/Object3DComponent.h
    scene/Transform.h
    scene/Octree.h
    asset/AssetLoader.h
    asset/ModelLoader.h
    material/Material.h
    material/DepthOnlyMaterial.h
    material/DistanceOnlyMaterial.h
    material/BasicMaterial.h
    material/BasicTexturedMaterial.h
    material/BasicTexturedLitMaterial.h
    material/BasicCubeMaterial.h
    material/StandardUniforms.h
    material/StandardAttributes.h
    material/Shader.h
    material/MaterialLibrary.h
    material/ShaderMaterialCharacteristic.h
    material/ShaderType.h
    Demo.h
    render/BaseRenderableContainer.h
    render/RenderableContainer.h
    render/Renderable.h
    render/BaseObjectRenderer.h
    render/ObjectRenderer.h
    render/Camera.h
    render/Renderer.h
    render/BaseRenderable.h
    render/MeshRenderer.h
    render/RenderState.h
    render/RenderTarget.h
    render/RenderTarget2D.h
    render/RenderTargetCube.h
    render/ViewDescriptor.h
    light/Light.h
    light/PointLight.h
    light/LightType.h
    light/LightCullType.h
    image/ImageLoader.h
    image/CubeTexture.h
    image/Texture2D.h
    image/Texture.h
    image/TextureAttr.h
    image/RawImage.h
    image/ImagePainter.h
    color/Color.h
    color/Color4Components.h
    color/IntColor.h
    util/Time.h
    util/String.h
    util/WeakPointer.h
    util/PersistentWeakPointer.h
    util/ValueIterator.h
    math/Math.h
    math/Quaternion.h
    math/Matrix4x4.h
    base/VectorStorage.h
    base/BaseVector.h
    base/BitMask.h
    common/gl.h
    common/assert.h
    common/types.h
    common/complextypes.h
    common/debug.h
    common/Exception.h
    GL/GraphicsGL.h
    GL/RendererGL.h
    GL/Texture2DGL.h
    GL/CubeTextureGL.h
    GL/ShaderGL.h
    GL/AttributeArrayGPUStorageGL.h
    GL/IndexBufferGL.h
    GL/RenderTargetGL.h
    GL/RenderTarget2DGL.h
    GL/RenderTargetCubeGL.h
    Graphics.h
    Engine.h)

set(SOURCE_FILES
    common/Debug.cpp
    asset/AssetLoader.cpp
    asset/ModelLoader.cpp
    filesys/FileSystem.cpp
    filesys/FileSystemIX.cpp
    gl3stub_wrapper.c
    image/ImageLoader.cpp
    image/RawImage.cpp
    image/Texture.cpp
    image/Texture2D.cpp
    image/TextureAttr.cpp
    image/CubeTexture.cpp
    image/PNGLoader.cpp
    image/ImagePainter.cpp
    geometry/AttributeArrayGPUStorage.cpp
    geometry/IndexBuffer.cpp
    geometry/Mesh.cpp
    geometry/Box3.cpp
    geometry/GeometryUtils.cpp
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
    render/ObjectRenderers.cpp
    render/RenderTarget.cpp
    render/RenderTarget2D.cpp
    render/RenderTargetCube.cpp
    light/Light.cpp
    light/PointLight.cpp
    material/Material.cpp
    material/DepthOnlyMaterial.cpp
    material/DistanceOnlyMaterial.cpp
    material/BasicMaterial.cpp
    material/BasicTexturedMaterial.cpp
    material/BasicTexturedLitMaterial.cpp
    material/BasicCubeMaterial.cpp
    material/Shader.cpp
    material/MaterialLibrary.cpp
    material/StandardUniforms.cpp
    material/StandardAttributes.cpp
    material/ShaderManager.cpp
    color/Color4Components.cpp
    math/Math.cpp
    math/Matrix4x4.cpp
    math/Quaternion.cpp
    util/Time.cpp
    util/String.cpp
    Engine.cpp
    Demo.cpp
    Graphics.cpp
    GL/GraphicsGL.cpp
    GL/RendererGL.cpp
    GL/Texture2DGL.cpp
    GL/CubeTextureGL.cpp
    GL/ShaderGL.cpp
    GL/IndexBufferGL.cpp
    GL/ShaderManagerGL.cpp
    GL/RenderTargetGL.cpp
    GL/RenderTarget2DGL.cpp
    GL/RenderTargetCubeGL.cpp)
    #RendererES3.cpp)

add_library(${EXECUTABLE_NAME} ${SOURCE_FILES})

target_link_libraries(${EXECUTABLE_NAME} ${OPENGL_LIBRARIES})

#include_directories(../../libpng/code)
#include_directories(../../libpng/build)
#target_link_libraries(${EXECUTABLE_NAME} ../../libpng/build/libpng.a)
target_link_libraries(${EXECUTABLE_NAME} png)

include_directories(../../assimp/build/include)
include_directories(../../assimp/include)
#target_link_libraries(${EXECUTABLE_NAME} ../../assimp/build/code/libassimp.a)
link_directories(../../assimp_dynamic/lib/)
target_link_libraries(${EXECUTABLE_NAME} assimp)

add_custom_target(MakeIncludeDir ALL COMMAND ${CMAKE_COMMAND} -E make_directory "include")
add_custom_target(MakeIncludeCoreDir ALL COMMAND ${CMAKE_COMMAND} -E make_directory "include/Core")

# do the copying
foreach(dir_i ${INCLUDE_DIRS})
    add_custom_command(
    TARGET ${EXECUTABLE_NAME}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND}
    ARGS -E make_directory "include/Core/${dir_i}"
)
endforeach(dir_i)

# do the copying
foreach(file_i ${INCLUDE_FILES})
    add_custom_command(
    TARGET ${EXECUTABLE_NAME}
    POST_BUILD
    # COMMENT "copying ${CMAKE_SOURCE_DIR}/${file_i} to  include/Core/${file_i}"
    COMMAND ${CMAKE_COMMAND}
    ARGS -E copy "${CMAKE_SOURCE_DIR}/${file_i}" "include/Core/${file_i}"
)
endforeach(file_i)

target_compile_definitions(core PRIVATE CORE_USE_PRIVATE_INCLUDES=1)

