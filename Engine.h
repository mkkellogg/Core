#pragma once

#include <functional>
#include <memory>

#include "scene/Scene.h"
#include "render/Camera.h"
#include "render/Renderer.h"
#include "render/RendererGL.h"
#include "image/ImageLoader.h"
#include "asset/AssetLoader.h"
#include "geometry/Vector4.h"
#include "render/BaseObjectRenderer.h"
#include "render/RenderableContainer.h"
#include "material/Material.h"
#include "util/ValidWeakPointer.h"

namespace Core {

  class Engine {

  public:

    enum class GLVersion {
      Two = 2,
      Three = 3,
    };

    Engine(GLVersion version);
    virtual ~Engine();

    void init();
    void update();
    void render();

    std::weak_ptr<Renderer> getRenderer();

    void setRenderSize(UInt32 width, UInt32 height, Bool updateViewport = true);
    void setRenderSize(UInt32 width, UInt32 height, UInt32 hOffset, UInt32 vOffset, UInt32 viewPortWidth, UInt32 viewPortHeight);
    void setViewport(UInt32 hOffset, UInt32 vOffset, UInt32 viewPortWidth, UInt32 viewPortHeight);

    void setActiveScene(std::weak_ptr<Scene> scene);
    std::weak_ptr<Scene> getActiveScene();
    std::weak_ptr<Scene> createScene();

    std::weak_ptr<Camera> createCamera();

    template <typename T>
    std::weak_ptr<typename std::enable_if<std::is_base_of<Object3D, T>::value, T>::type> createObject3D() {
      std::shared_ptr<T> objPtr = std::shared_ptr<T>(new T());
      this->sceneObjects.push_back(objPtr);
      return objPtr;
    }

    template <typename T, typename R>
    std::weak_ptr<typename std::enable_if<std::is_base_of<ObjectRenderer<R>, T>::value, T>::type> 
    createRenderer(std::weak_ptr<Material> material, std::weak_ptr<RenderableContainer<R>> owner) {
      std::shared_ptr<T> rendererPtr = std::shared_ptr<T>(new T(material, owner));
      ValidWeakPointer<RenderableContainer<R>> ownerPtr(owner);
      ownerPtr->setRenderer(rendererPtr);
      return rendererPtr;
    }

    template <typename T>
    std::weak_ptr<typename std::enable_if<std::is_base_of<Material, T>::value, T>::type> createMaterial() {
      std::shared_ptr<T> materialPtr = std::shared_ptr<T>(new T());
      this->materials.push_back(materialPtr);
      return materialPtr;
    }

    void setImageLoader(std::weak_ptr<ImageLoader> imageLoader);
    std::weak_ptr<ImageLoader> getImageLoader();
    void setAssetLoader(std::weak_ptr<AssetLoader> assetLoader);
    std::weak_ptr<AssetLoader> getAssetLoader();

    void onUpdate(std::function<void(Engine&)> func);

  private:
    GLVersion glVersion;
    std::vector<std::shared_ptr<Scene>> scenes;
    std::shared_ptr<Scene> activeScene;
    std::vector<std::shared_ptr<Camera>> cameras;
    std::vector<std::shared_ptr<Object3D>> sceneObjects;
    std::vector<std::shared_ptr<Material>> materials;

    std::weak_ptr<ImageLoader> imageLoader;
    std::weak_ptr<AssetLoader> assetLoader;
    std::shared_ptr<Renderer> renderer;
    std::vector<std::function<void(Engine&)>> updateCallbacks;

    void cleanup();

  };
}
