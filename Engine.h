//
// Created by Mark Kellogg on 12/14/17.
//

#ifndef MPM_ENGINE_H
#define MPM_ENGINE_H

#include <functional>
#include "scene/Scene.h"
#include "render/Camera.h"
#include "render/Renderer.h"
#include "render/RendererES2.h"
#include "image/ImageLoader.h"
#include "asset/AssetLoader.h"

namespace Core {

  class Engine {

  public:

    enum class GLVersion {
      Two = 2,
      Three = 3,
    };

  private:

    GLVersion glVersion;
    std::shared_ptr<Scene> scene;
    std::shared_ptr<Camera> camera;
    std::shared_ptr<ImageLoader> imageLoader;
    std::shared_ptr<AssetLoader> assetLoader;
    Renderer* renderer;
    UInt32 screenWidth;
    UInt32 screenHeight;

    std::vector<std::function<void(Engine&)>> updateCallbacks;

    void cleanup();

  public:

    Engine(GLVersion version);
    virtual ~Engine();

    void init();
    void update();
    void render();

    void resize(UInt32 width, UInt32 height);
    void setScene(std::shared_ptr<Scene> scene);
    std::shared_ptr<Scene> getScene();
    void setCamera(std::shared_ptr<Camera> camera);
    std::shared_ptr<Camera> getCamera();
    void setImageLoader(std::shared_ptr<ImageLoader> imageLoader);
    std::shared_ptr<ImageLoader> getImageLoader();
    void setAssetLoader(std::shared_ptr<AssetLoader> assetLoader);
    std::shared_ptr<AssetLoader> getAssetLoader();

    void onUpdate(std::function<void(Engine&)> func);

  };
}

#endif //MPM_ENGINE_H
