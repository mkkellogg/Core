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

    void setImageLoader(std::shared_ptr<ImageLoader> imageLoader);
    std::shared_ptr<ImageLoader> getImageLoader();
    void setAssetLoader(std::shared_ptr<AssetLoader> assetLoader);
    std::shared_ptr<AssetLoader> getAssetLoader();

    void onUpdate(std::function<void(Engine&)> func);

  private:
    GLVersion glVersion;
    std::vector<std::shared_ptr<Scene>> scenes;
    std::shared_ptr<Scene> activeScene;
    std::shared_ptr<ImageLoader> imageLoader;
    std::shared_ptr<AssetLoader> assetLoader;
    std::shared_ptr<Renderer> renderer;
    std::vector<std::function<void(Engine&)>> updateCallbacks;

    void cleanup();

  };
}
