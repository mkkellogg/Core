//
// Created by Mark Kellogg on 12/14/17.
//

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
    static char glString[512];
    const char* v = (const char*)glGetString(s);
    sprintf(glString, "GL %s: %s\n", name, v);
    DebugPrintMessage(glString);
  }

  Engine::Engine(GLVersion version):
      glVersion(version), renderer(nullptr), camera(nullptr), screenWidth(0), screenHeight(0) {

  }

  Engine::~Engine() {
    cleanup();
  }

  void Engine::cleanup() {
    if (renderer != nullptr) {
      delete renderer;
      renderer = nullptr;
    }
  }

  void Engine::init() {
    cleanup();

    printGlString("Version", GL_VERSION);
    printGlString("Vendor", GL_VENDOR);
    printGlString("Renderer", GL_RENDERER);
    printGlString("Extensions", GL_EXTENSIONS);

    /*UInt32 maxGL = 0;
    const char* versionStr = (const char*)glGetString(GL_VERSION);
    if (strstr(versionStr, "OpenGL ES 3.") && gl3stubInit()) {
      maxGL = 3;
    }
    else if (strstr(versionStr, "OpenGL ES 2.")) {
      maxGL = 2;
    }
    else {
      maxGL = 1;
    }

    if (glVersion == GLVersion::Three) {
      // Currently not supported...
      DebugPrintError("OpenGL ES 3.0 not supported!");
      exit(-1);
    }
    else if (glVersion == GLVersion::Two) {
      if (maxGL >= 2) {
        renderer = Core::RendererES2::createRenderer();
        DebugPrintError("Using OpenGL ES 2.0 renderer");
      }
      else {
        DebugPrintError("Unable to start desired OpenGL ES 2.0 renderer!");
        exit(-1);
      }
    }
    else {
      DebugPrintError("Unsupported OpenGL ES version");
      exit(-1);
    }*/

    glClearColor(1.0, 0, 0, 1);
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
    if (this->scene) {
      this->renderer->render(this->scene, this->camera);
    }
  }

  void Engine::resize(UInt32 width, UInt32 height) {
    glViewport(0, 0, width, height);
    this->screenWidth = width;
    this->screenHeight = height;
    if (this->camera) {
      this->camera->setAspectRatioFromDimensions(width, height);
    }
  }

  void Engine::setScene(std::shared_ptr<Scene> scene) {
    this->scene = scene;
  }

  std::shared_ptr<Scene> Engine::getScene() {
    return this->scene;
  }

  void Engine::setCamera(std::shared_ptr<Camera> camera) {
    this->camera = camera;
  }

  std::shared_ptr<Camera> Engine::getCamera() {
    return this->camera;
  }

  void Engine::setImageLoader(std::shared_ptr<ImageLoader> imageLoader) {
    this->imageLoader = imageLoader;
  }

  std::shared_ptr<ImageLoader> Engine::getImageLoader() {
    return this->imageLoader;
  }

  void Engine::setAssetLoader(std::shared_ptr<AssetLoader> assetLoader) {
    this->assetLoader = assetLoader;
  }

  std::shared_ptr<AssetLoader> Engine::getAssetLoader() {
    return this->assetLoader;
  }

  void Engine::onUpdate(std::function<void(Engine&)> func) {
    this->updateCallbacks.push_back(func);
  }
}