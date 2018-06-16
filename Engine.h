#pragma once

#include <functional>
#include <memory>

#include "GL/GraphicsGL.h"
#include "scene/Scene.h"
#include "render/Camera.h"
#include "render/Renderer.h"
#include "asset/AssetLoader.h"
#include "geometry/Vector4.h"
#include "render/BaseObjectRenderer.h"
#include "render/RenderableContainer.h"
#include "material/Material.h"
#include "image/Texture2D.h"
#include "image/CubeTexture.h"
#include "image/TextureAttr.h"
#include "geometry/Mesh.h"
#include "util/WeakPointer.h"

namespace Core {

  // forward declarations
  class ImageLoader;

  class Engine {

  public:

    Engine();
    virtual ~Engine();

    void init();
    void update();

    void render();
    void setRenderSize(UInt32 width, UInt32 height, Bool updateViewport = true);
    void setRenderSize(UInt32 width, UInt32 height, UInt32 hOffset, UInt32 vOffset, UInt32 viewPortWidth, UInt32 viewPortHeight);
    void setViewport(UInt32 hOffset, UInt32 vOffset, UInt32 viewPortWidth, UInt32 viewPortHeight);

    WeakPointer<Graphics> getGraphicsSystem();

    void setActiveScene(std::weak_ptr<Scene> scene);
    WeakPointer<Scene> getActiveScene();
    WeakPointer<Scene> createScene();

    WeakPointer<Camera> createCamera(std::weak_ptr<Object3D> owner);

    template <typename T = Object3D>
    WeakPointer<typename std::enable_if<std::is_base_of<Object3D, T>::value, T>::type> createObject3D() {
      std::shared_ptr<T> objPtr = std::shared_ptr<T>(new T());
      this->sceneObjects.push_back(objPtr);
      return objPtr;
    }

    WeakPointer<Mesh> createMesh(UInt32 size, Bool indexed);

    template <typename T, typename R>
    WeakPointer<typename std::enable_if<std::is_base_of<ObjectRenderer<R>, T>::value, T>::type> 
    createRenderer(std::weak_ptr<Material> material, std::weak_ptr<RenderableContainer<R>> owner) {
      std::shared_ptr<T> objectRenderer = std::shared_ptr<T>(new T(this->graphics, material, owner));
      WeakPointer<RenderableContainer<R>> ownerPtr(owner);
      ownerPtr->setRenderer(objectRenderer);
      objectRenderers.push_back(objectRenderer);
      return objectRenderer;
    }

    template <typename T>
    WeakPointer<typename std::enable_if<std::is_base_of<Material, T>::value, T>::type> createMaterial() {
      std::shared_ptr<T> materialPtr = std::shared_ptr<T>(new T(this->graphics));
      this->materials.push_back(materialPtr);
      return materialPtr;
    }

    WeakPointer<Texture2D> createTexture2D(const TextureAttributes& attributes);
    WeakPointer<CubeTexture> createCubeTexture(const TextureAttributes& attributes);

    void setImageLoader(std::weak_ptr<ImageLoader> imageLoader);
    std::weak_ptr<ImageLoader> getImageLoader();
    void setAssetLoader(std::weak_ptr<AssetLoader> assetLoader);
    std::weak_ptr<AssetLoader> getAssetLoader();

    void onUpdate(std::function<void(Engine&)> func);

  private:
    
    std::shared_ptr<Graphics> graphics;

    std::vector<std::shared_ptr<Scene>> scenes;
    std::shared_ptr<Scene> activeScene;
    std::vector<std::shared_ptr<Camera>> cameras;
    std::vector<std::shared_ptr<Object3D>> sceneObjects;
    std::vector<std::shared_ptr<Material>> materials;
    std::vector<std::shared_ptr<Texture>> textures;
    std::vector<std::shared_ptr<BaseObjectRenderer>> objectRenderers;
    std::vector<std::shared_ptr<Mesh>> meshes;

    std::weak_ptr<ImageLoader> imageLoader;
    std::weak_ptr<AssetLoader> assetLoader;
    std::vector<std::function<void(Engine&)>> updateCallbacks;

    void cleanup();

  };
}
