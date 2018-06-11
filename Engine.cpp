#include <stdlib.h>
#include <string.h>
#include <new>
#include "Engine.h"
#include "common/debug.h"
#include "math/Quaternion.h"
#include "math/Math.h"
#include "geometry/Vector3.h"
#include "util/Time.h"
#include "util/ValidWeakPointer.h"

namespace Core {

  Engine::Engine() {

  }

  Engine::~Engine() {
    cleanup();
  }

  void Engine::cleanup() {

  }

  void Engine::init() {
    cleanup();
    std::shared_ptr<GraphicsGL> graphicsSystem(new GraphicsGL(GraphicsGL::GLVersion::Three));
    this->graphics = std::static_pointer_cast<Graphics>(graphicsSystem);
    this->graphics->init();
  }

  void Engine::update() {
    Time::update();
    for(auto func: this->updateCallbacks) {
      func(*this);
    }
  }

  void Engine::render() {
    ValidWeakPointer<Renderer> renderer(this->graphics->getRenderer());
    if (this->activeScene) {
      renderer->render(this->activeScene);
    }
  }

  void Engine::setRenderSize(UInt32 width, UInt32 height, Bool updateViewport) {
    ValidWeakPointer<Renderer> renderer(this->graphics->getRenderer());
    if (ValidWeakPointer<Renderer>::isInitialized(renderer)) {
      renderer->setRenderSize(width, height, updateViewport);
    }
  }

  void Engine::setRenderSize(UInt32 width, UInt32 height, UInt32 hOffset, UInt32 vOffset, UInt32 viewPortWidth, UInt32  viewPortHeight) {
    ValidWeakPointer<Renderer> renderer(this->graphics->getRenderer());
    if (ValidWeakPointer<Renderer>::isInitialized(renderer)) {
      renderer->setRenderSize(width, height, hOffset, vOffset, viewPortWidth, viewPortHeight);
    }
  }

  void Engine::setViewport(UInt32 hOffset, UInt32 vOffset, UInt32 viewPortWidth, UInt32 viewPortHeight) {
    ValidWeakPointer<Renderer> renderer(this->graphics->getRenderer());
    if (ValidWeakPointer<Renderer>::isInitialized(renderer)) {
      renderer->setViewport(hOffset, vOffset, viewPortWidth, viewPortHeight);
    }
  }

  void Engine::setActiveScene(std::weak_ptr<Scene> scene) {
    this->activeScene = scene.lock();
  }

  std::weak_ptr<Scene> Engine::getActiveScene() {
    return this->activeScene;
  }

  std::weak_ptr<Scene> Engine::createScene() {
    std::weak_ptr<Object3D> newRoot = this->createObject3D<Object3D>();
    std::shared_ptr<Scene> newScene = std::shared_ptr<Scene>(new Scene(newRoot));
    this->scenes.push_back(newScene);
    return newScene;
  }

  std::weak_ptr<Camera> Engine::createCamera() {
    std::shared_ptr<Camera> newCamera = std::shared_ptr<Camera>(new Camera());
    this->cameras.push_back(newCamera);
    return newCamera;
  }

  void Engine::setImageLoader(std::weak_ptr<ImageLoader> imageLoader) {
    this->imageLoader = imageLoader;
  }

  std::weak_ptr<ImageLoader> Engine::getImageLoader() {
    return this->imageLoader;
  }

  void Engine::setAssetLoader(std::weak_ptr<AssetLoader> assetLoader) {
    this->assetLoader = assetLoader;
  }

  std::weak_ptr<AssetLoader> Engine::getAssetLoader() {
    return this->assetLoader;
  }

  void Engine::onUpdate(std::function<void(Engine&)> func) {
    this->updateCallbacks.push_back(func);
  }
}