#include <stdlib.h>
#include <string.h>
#include <new>
#include "Engine.h"
#include "common/debug.h"
#include "math/Quaternion.h"
#include "math/Math.h"
#include "geometry/Vector3.h"
#include "util/Time.h"

#if !defined(DYNAMIC_ES3)
static GLboolean gl3stubInit() {
  return GL_TRUE;
}
#endif

namespace Core {

  static void printGlString(const char* name, GLenum s) {
    const char* v = (const char*)glGetString(s);
    Debug::PrintMessage("GL %s: %s\n", name, v);
  }

  Engine::Engine(GLVersion version):
      glVersion(version), renderer(nullptr) {

  }

  Engine::~Engine() {
    cleanup();
  }

  void Engine::cleanup() {

  }

  void Engine::init() {
    cleanup();

    printGlString("Version", GL_VERSION);
    printGlString("Vendor", GL_VENDOR);
    printGlString("Renderer", GL_RENDERER);
    printGlString("Extensions", GL_EXTENSIONS);

    UInt32 maxGL = 0;
    const char* versionStr = (const char*)glGetString(GL_VERSION);

    renderer = std::shared_ptr<Renderer>(Core::RendererGL::createRenderer());

    glClearColor(0, 0, 0, 1);
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LEQUAL);

  }

  void Engine::update() {
    Time::update();
    for(auto func: this->updateCallbacks) {
      func(*this);
    }
  }

  void Engine::render() {
    if (this->activeScene) {
      this->renderer->render(this->activeScene);
    }
  }

  std::weak_ptr<Renderer> Engine::getRenderer() {
    return renderer;
  }

  void Engine::setRenderSize(UInt32 width, UInt32 height, Bool updateViewport) {
    if (this->renderer) {
      this->renderer->setRenderSize(width, height, updateViewport);
    }
  }

  void Engine::setRenderSize(UInt32 width, UInt32 height, UInt32 hOffset, UInt32 vOffset, UInt32 viewPortWidth, UInt32  viewPortHeight) {
    if (this->renderer) {
      this->renderer->setRenderSize(width, height, hOffset, vOffset, viewPortWidth, viewPortHeight);
    }
  }

  void Engine::setViewport(UInt32 hOffset, UInt32 vOffset, UInt32 viewPortWidth, UInt32 viewPortHeight) {
    if (this->renderer) {
      this->renderer->setViewport(hOffset, vOffset, viewPortWidth, viewPortHeight);
    }
  }

  void Engine::setActiveScene(std::weak_ptr<Scene> scene) {
    this->activeScene = scene.lock();
  }

  std::weak_ptr<Scene> Engine::getActiveScene() {
    return this->activeScene;
  }

  std::weak_ptr<Scene> Engine::createScene() {
    std::shared_ptr<Scene> newScene = std::shared_ptr<Scene>(new Scene());
    this->scenes.push_back(newScene);
    return newScene;
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