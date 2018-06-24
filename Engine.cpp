#include <stdlib.h>
#include <string.h>
#include <new>

#include "Engine.h"
#include "common/debug.h"
#include "geometry/Vector3.h"
#include "math/Math.h"
#include "math/Quaternion.h"
#include "util/Time.h"
#include "GL/GraphicsGL.h"
#include "render/BaseObjectRenderer.h"
#include "scene/Scene.h"
#include "render/Camera.h"
#include "geometry/Mesh.h"
#include "image/CubeTexture.h"
#include "material/BasicMaterial.h"
#include "material/BasicCubeMaterial.h"
#include "material/BasicTexturedMaterial.h"

namespace Core {

    Engine::Engine(): modelLoader(*this) {
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

        WeakPointer<BasicMaterial> basicMaterial = this->createMaterial<BasicMaterial>();
        WeakPointer<BasicTexturedMaterial> basicTexturedMaterial = this->createMaterial<BasicTexturedMaterial>();
        WeakPointer<BasicCubeMaterial> basicCubeMaterial = this->createMaterial<BasicCubeMaterial>();

        LongMask materialAttributes;
        materialAttributes = LongMaskUtil::createMask();
        LongMaskUtil::setBit(&materialAttributes, (Int16)ShaderMaterialCharacteristic::VertexNormals);
        this->materialLibrary.addEntry(materialAttributes, basicMaterial);

        materialAttributes = LongMaskUtil::createMask();
        LongMaskUtil::setBit(&materialAttributes, (Int16)ShaderMaterialCharacteristic::DiffuseTextured);
        LongMaskUtil::setBit(&materialAttributes, (Int16)ShaderMaterialCharacteristic::VertexNormals);
        this->materialLibrary.addEntry(materialAttributes, basicTexturedMaterial);

        materialAttributes = LongMaskUtil::createMask();
        LongMaskUtil::setBit(&materialAttributes, (Int16)ShaderMaterialCharacteristic::CubeTextured);
        LongMaskUtil::setBit(&materialAttributes, (Int16)ShaderMaterialCharacteristic::VertexNormals);
        this->materialLibrary.addEntry(materialAttributes, basicCubeMaterial);
    }

    void Engine::update() {
        Time::update();
        if (this->updateCallbacks.size() > 0) {
            for (auto func : this->updateCallbacks) {
                func(*this);
            }
            this->updateCallbacks.clear();
        }
        for (auto func : this->persistentUpdateCallbacks) {
            func(*this);
        }
    }

    void Engine::render() {
        if (this->activeScene) {
            this->graphics->render(this->activeScene);
            if (this->renderCallbacks.size() > 0) {
                for (auto func : this->renderCallbacks) {
                    func(*this);
                }
                this->renderCallbacks.clear();
            }
            for (auto func : this->persistentRenderCallbacks) {
                func(*this);
            }
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

    MaterialLibrary& Engine::getMaterialLibrary() {
        return this->materialLibrary;
    }

    ModelLoader& Engine::getModelLoader() {
        return this->modelLoader;
    }

    WeakPointer<Graphics> Engine::getGraphicsSystem() {
        return this->graphics;
    }

    void Engine::setActiveScene(WeakPointer<Scene> scene) {
        this->activeScene = scene.lock();
    }

    WeakPointer<Scene> Engine::getActiveScene() {
        return this->activeScene;
    }

    WeakPointer<Scene> Engine::createScene() {
        WeakPointer<Object3D> newRoot = this->createObject3D<Object3D>();
        std::shared_ptr<Scene> newScene = std::shared_ptr<Scene>(new Scene(newRoot));
        this->scenes.push_back(newScene);
        return newScene;
    }

    WeakPointer<Mesh> Engine::createMesh(UInt32 size, Bool indexed) {
        std::shared_ptr<Mesh> newMesh = std::shared_ptr<Mesh>(new Mesh(this->graphics, size, indexed));
        this->meshes.push_back(newMesh);
        return newMesh;
    }

    WeakPointer<Camera> Engine::createCamera(WeakPointer<Object3D> owner) {
        std::shared_ptr<Camera> newCamera = std::shared_ptr<Camera>(new Camera(owner));
        this->cameras.push_back(newCamera);
        WeakPointer<Camera> cameraPtr(newCamera);
        owner->addComponent(cameraPtr);
        return cameraPtr;
    }

    WeakPointer<Texture2D> Engine::createTexture2D(const TextureAttributes& attributes) {
        return this->graphics->createTexture2D(attributes);
    }

    WeakPointer<CubeTexture> Engine::createCubeTexture(const TextureAttributes& attributes) {
        return this->graphics->createCubeTexture(attributes);
    }

    void Engine::setImageLoader(WeakPointer<ImageLoader> imageLoader) {
        this->imageLoader = imageLoader;
    }

    WeakPointer<ImageLoader> Engine::getImageLoader() {
        return this->imageLoader;
    }

    void Engine::setAssetLoader(WeakPointer<AssetLoader> assetLoader) {
        this->assetLoader = assetLoader;
    }

    WeakPointer<AssetLoader> Engine::getAssetLoader() {
        return this->assetLoader;
    }

    void Engine::onUpdate(LifecycleEventCallback func, Bool persistent) {
        if (!persistent) this->updateCallbacks.push_back(func);
        else this->persistentUpdateCallbacks.push_back(func);
    }

     void Engine::onRender(LifecycleEventCallback func, Bool persistent) {
        if (!persistent) this->renderCallbacks.push_back(func);
        else this->persistentRenderCallbacks.push_back(func);
    }
}