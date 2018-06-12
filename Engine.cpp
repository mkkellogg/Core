#include "Engine.h"
#include <stdlib.h>
#include <string.h>
#include <new>
#include "common/debug.h"
#include "geometry/Vector3.h"
#include "math/Math.h"
#include "math/Quaternion.h"
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
        for (auto func : this->updateCallbacks) {
            func(*this);
        }
    }

    void Engine::render() {
        if (this->activeScene) {
            this->graphics->render(this->activeScene);
        }
    }

    void Engine::setRenderSize(UInt32 width, UInt32 height, Bool updateViewport) {
        this->graphics->setRenderSize(width, height, updateViewport);
    }

    void Engine::setRenderSize(UInt32 width, UInt32 height, UInt32 hOffset, UInt32 vOffset, UInt32 viewPortWidth, UInt32 viewPortHeight) {
        this->graphics->setRenderSize(width, height, hOffset, vOffset, viewPortWidth, viewPortHeight);
    }

    void Engine::setViewport(UInt32 hOffset, UInt32 vOffset, UInt32 viewPortWidth, UInt32 viewPortHeight) {
        this->graphics->setViewport(hOffset, vOffset, viewPortWidth, viewPortHeight);
    }

    std::shared_ptr<Graphics> Engine::getGraphicsSystem() {
        return this->graphics;
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

    std::weak_ptr<Texture2D> Engine::createTexture2D() {
        return this->graphics->createTexture2D();
    }

    std::weak_ptr<CubeTexture> Engine::createCubeTexture() {
        return this->graphics->createCubeTexture();
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