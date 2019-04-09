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
#include "render/ReflectionProbe.h"
#include "scene/Scene.h"
#include "scene/Object3D.h"
#include "render/Camera.h"
#include "geometry/Mesh.h"
#include "image/CubeTexture.h"
#include "material/BasicMaterial.h"
#include "material/BasicCubeMaterial.h"
#include "material/BasicTexturedMaterial.h"
#include "material/BasicTexturedLitMaterial.h"
#include "material/StandardPhysicalMaterial.h"
#include "material/AmbientPhysicalMaterial.h"

namespace Core {

    std::shared_ptr<Engine> Engine::_instance;

    WeakPointer<Engine> Engine::instance() {
        if (!_instance) {
            _instance = std::shared_ptr<Engine>(new Engine());
            _instance->init();
        }
        return _instance;
    }

    Engine::Engine(): modelLoader() {
    }

    Engine::~Engine() {
        cleanup();
    }

    void Engine::cleanup() {
    }

    void Engine::init() {

        cleanup();

        // TODO: make this configurable so that it is not hard-coded to use OpenGL
        std::shared_ptr<GraphicsGL> graphicsSystem(new GraphicsGL(GraphicsGL::GLVersion::Three));
        this->graphics = std::static_pointer_cast<Graphics>(graphicsSystem);
        this->graphics->init();

        WeakPointer<BasicMaterial> basicMaterial = this->createMaterial<BasicMaterial>();
        WeakPointer<BasicTexturedMaterial> basicTexturedMaterial = this->createMaterial<BasicTexturedMaterial>();
        WeakPointer<BasicTexturedLitMaterial> basicTexturedLitMaterial = this->createMaterial<BasicTexturedLitMaterial>();
        WeakPointer<StandardPhysicalMaterial> standardPhysicalMaterial = this->createMaterial<StandardPhysicalMaterial>();
        WeakPointer<StandardPhysicalMaterial> ambientPhysicalMaterial = this->createMaterial<AmbientPhysicalMaterial>();
        WeakPointer<BasicCubeMaterial> basicCubeMaterial = this->createMaterial<BasicCubeMaterial>();

        LongMask materialAttributes;
        materialAttributes = LongMaskUtil::createMask();
        LongMaskUtil::setBit(&materialAttributes, (Int16)ShaderMaterialCharacteristic::VertexNormals);
        LongMaskUtil::setBit(&materialAttributes, (Int16)ShaderMaterialCharacteristic::VertexColors);
        this->materialLibrary.addEntry(materialAttributes, basicMaterial);

        materialAttributes = LongMaskUtil::createMask();
        LongMaskUtil::setBit(&materialAttributes, (Int16)ShaderMaterialCharacteristic::AlbedoMapped);
        LongMaskUtil::setBit(&materialAttributes, (Int16)ShaderMaterialCharacteristic::VertexNormals);
        LongMaskUtil::setBit(&materialAttributes, (Int16)ShaderMaterialCharacteristic::VertexColors);
        this->materialLibrary.addEntry(materialAttributes, basicTexturedMaterial);

        materialAttributes = LongMaskUtil::createMask();
        LongMaskUtil::setBit(&materialAttributes, (Int16)ShaderMaterialCharacteristic::AlbedoMapped);
        LongMaskUtil::setBit(&materialAttributes, (Int16)ShaderMaterialCharacteristic::VertexNormals);
        LongMaskUtil::setBit(&materialAttributes, (Int16)ShaderMaterialCharacteristic::VertexColors);
        LongMaskUtil::setBit(&materialAttributes, (Int16)ShaderMaterialCharacteristic::NormalMapped);
        LongMaskUtil::setBit(&materialAttributes, (Int16)ShaderMaterialCharacteristic::Lit);
        this->materialLibrary.addEntry(materialAttributes, ambientPhysicalMaterial);

        materialAttributes = LongMaskUtil::createMask();
        LongMaskUtil::setBit(&materialAttributes, (Int16)ShaderMaterialCharacteristic::AlbedoMapped);
        LongMaskUtil::setBit(&materialAttributes, (Int16)ShaderMaterialCharacteristic::VertexNormals);
        LongMaskUtil::setBit(&materialAttributes, (Int16)ShaderMaterialCharacteristic::VertexColors);
        LongMaskUtil::setBit(&materialAttributes, (Int16)ShaderMaterialCharacteristic::NormalMapped);
        LongMaskUtil::setBit(&materialAttributes, (Int16)ShaderMaterialCharacteristic::Lit);
        LongMaskUtil::setBit(&materialAttributes, (Int16)ShaderMaterialCharacteristic::Physical);
        this->materialLibrary.addEntry(materialAttributes, standardPhysicalMaterial);

        materialAttributes = LongMaskUtil::createMask();
        LongMaskUtil::setBit(&materialAttributes, (Int16)ShaderMaterialCharacteristic::CubeTextured);
        LongMaskUtil::setBit(&materialAttributes, (Int16)ShaderMaterialCharacteristic::VertexNormals);
        this->materialLibrary.addEntry(materialAttributes, basicCubeMaterial);
    }

    void Engine::update() {
        Time::update();
        if (this->updateCallbacks.size() > 0) {
            for (auto func : this->updateCallbacks) {
                func();
            }
            this->updateCallbacks.clear();
        }
        for (auto func : this->persistentUpdateCallbacks) {
            func();
        }
    }

    void Engine::resolveRenderCallbacks(std::vector<LifecycleEventCallback>& oneTime, const std::vector<LifecycleEventCallback>& persistent) {
        if (oneTime.size() > 0) {
            for (auto func : oneTime) {
                func();
            }
            oneTime.clear();
        }
        for (auto func : persistent) {
            func();
        }
    }

    void Engine::render() {
        if (this->activeScene) {
            this->graphics->preRender();
            this->resolveRenderCallbacks(this->preRenderCallbacks, this->persistentPreRenderCallbacks);
            if (this->graphics->getRenderer()) {
               this->graphics->getRenderer()->renderScene(this->activeScene);
            }
            this->resolveRenderCallbacks(this->postRenderCallbacks, this->persistentPostRenderCallbacks);
            this->graphics->postRender();
        }
    }

    void Engine::setRenderSize(UInt32 width, UInt32 height, Bool updateViewport) {
        this->graphics->setRenderSize(width, height, updateViewport);
    }

    void Engine::setRenderSize(UInt32 width, UInt32 height, UInt32 hOffset, UInt32 vOffset, UInt32 viewPortWidth, UInt32 viewPortHeight) {
        this->graphics->setRenderSize(width, height, hOffset, vOffset, viewPortWidth, viewPortHeight);
    }

    void Engine::setDefaultViewport(UInt32 hOffset, UInt32 vOffset, UInt32 viewPortWidth, UInt32 viewPortHeight) {
        this->graphics->setDefaultViewport(hOffset, vOffset, viewPortWidth, viewPortHeight);
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
        Scene* newScenePtr = new(std::nothrow) Scene(newRoot);
        if (newScenePtr == nullptr) {
            throw AllocationException("Engine::createScene -> Unable to allocate new Scene");
        }
        std::shared_ptr<Scene> newScene = std::shared_ptr<Scene>(newScenePtr);
        this->scenes.push_back(newScene);
        return newScene;
    }

    WeakPointer<Mesh> Engine::createMesh(UInt32 size, UInt32 indexCount) {
        Mesh* newMeshPtr = new(std::nothrow) Mesh(this->graphics, size, indexCount);
        if (newMeshPtr == nullptr) {
            throw AllocationException("Engine::createMesh -> Unable to allocate new Mesh");
        }
        std::shared_ptr<Mesh> newMesh = std::shared_ptr<Mesh>(newMeshPtr);
        newMesh->init();
        this->meshes.push_back(newMesh);
        return newMesh;
    }

    WeakPointer<Camera> Engine::createPerspectiveCamera(WeakPointer<Object3D> owner, Real fov, Real aspect, Real near, Real far) {
        std::shared_ptr<Camera> newCamera = std::shared_ptr<Camera>(Camera::createPerspectiveCamera(owner, fov, aspect, near, far));
        this->cameras.push_back(newCamera);
        WeakPointer<Camera> cameraPtr(newCamera);
        owner->addComponent(cameraPtr);
        return cameraPtr;
    }

    WeakPointer<Camera> Engine::createOrthographicCamera(WeakPointer<Object3D> owner, Real top, Real bottom, Real left, Real right, Real near, Real far) {
        std::shared_ptr<Camera> newCamera = std::shared_ptr<Camera>(Camera::createOrthographicCamera(owner, top, bottom, left, right, near, far));
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

    void Engine::destroyTexture2D(WeakPointer<Texture2D> texture) {
        this->graphics->destroyTexture2D(texture);
    }

    void Engine::destroyCubeTexture(WeakPointer<CubeTexture> texture) {
        this->graphics->destroyCubeTexture(texture);
    }

    WeakPointer<ReflectionProbe> Engine::createReflectionProbe(WeakPointer<Object3D> owner) {
        ReflectionProbe* newReflectionProbePtr = new(std::nothrow) ReflectionProbe(owner);
        if (newReflectionProbePtr == nullptr) {
            throw AllocationException("Engine::createReflectionProbe -> Unable to allocate new ReflectionProbe");
        }
        std::shared_ptr<ReflectionProbe> newReflectionProbe = std::shared_ptr<ReflectionProbe>(newReflectionProbePtr);
        newReflectionProbe->init();
        this->reflectionProbes.push_back(newReflectionProbe);
        WeakPointer<ReflectionProbe> probePtr(newReflectionProbe);
        owner->addComponent(probePtr);
        return newReflectionProbe;
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

    void Engine::setDefaultRenderTargetToCurrent() {
        this->graphics->setDefaultRenderTargetToCurrent();
    }

    void Engine::onUpdate(LifecycleEventCallback func, Bool persistent) {
        if (!persistent) this->updateCallbacks.push_back(func);
        else this->persistentUpdateCallbacks.push_back(func);
    }

    void Engine::onPreRender(LifecycleEventCallback func, Bool persistent) {
        if (!persistent) this->preRenderCallbacks.push_back(func);
        else this->persistentPreRenderCallbacks.push_back(func);
    }

    void Engine::onPostRender(LifecycleEventCallback func, Bool persistent) {
        if (!persistent) this->postRenderCallbacks.push_back(func);
        else this->persistentPostRenderCallbacks.push_back(func);
    }
}