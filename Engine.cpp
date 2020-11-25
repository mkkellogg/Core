#include <stdlib.h>
#include <string.h>
#include <new>

#include "Engine.h"
#include "common/debug.h"
#include "util/Time.h"
#include "GL/GraphicsGL.h"
#include "geometry/Vector3.h"
#include "math/Math.h"
#include "math/Quaternion.h"
#include "animation/AnimationManager.h"
#include "animation/Skeleton.h"
#include "animation/VertexBoneMap.h"
#include "render/BaseObject3DRenderer.h"
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
#include "particles/ParticleSystemManager.h"
#include "particles/ParticleSystem.h"
#include "particles/operator/BasicParticleStateOperator.h"

namespace Core {

    std::shared_ptr<Engine> Engine::_instance;
    Bool Engine::_shuttingDown = false;

    WeakPointer<Engine> Engine::instance() {
        errorIfShuttingDown();
        if (!_instance) {
            _instance = std::shared_ptr<Engine>(new Engine());
            _instance->init();
        }
        return _instance;
    }

    Bool Engine::isShuttingDown() {
        return _shuttingDown;
    }

    void Engine::errorIfShuttingDown() {
        if(_shuttingDown) {
            throw Exception("Cannot access engine during shutdown.");
        }
    }

    Engine::Engine(): modelLoader() {
    }

    Engine::~Engine() {
        _shuttingDown = true;
    }
    
    void Engine::init() {

        // TODO: make this configurable so that it is not hard-coded to use OpenGL
        std::shared_ptr<GraphicsGL> graphicsSystem(new GraphicsGL(GraphicsGL::GLVersion::Three));
        this->graphics = std::static_pointer_cast<Graphics>(graphicsSystem);
        this->graphics->init();

        this->animationManager = std::shared_ptr<AnimationManager>(new AnimationManager());
        this->particleSystemManager = std::shared_ptr<ParticleSystemManager>(new ParticleSystemManager());

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
        static std::vector<LifecycleEventCallback> tempUpdateCallbacks;
        static std::vector<LifecycleEventCallback> tempPersistentUpdateCallbacks;
        Time::update();
        this->animationManager->update();
        this->particleSystemManager->update();
        if (this->updateCallbacks.size() > 0) {
            
            for (auto func : this->updateCallbacks) {
               tempUpdateCallbacks.push_back(func);
            }
            this->updateCallbacks.clear();

            for (auto func : tempUpdateCallbacks) {
                func();
            }
            tempUpdateCallbacks.clear();
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
        errorIfShuttingDown();
        return this->graphics;
    }

    WeakPointer<AnimationManager> Engine::getAnimationManager() {
        errorIfShuttingDown();
        return this->animationManager;
    }

    WeakPointer<ParticleSystemManager> Engine::getParticleSystemManager() {
        errorIfShuttingDown();
        return this->particleSystemManager;
    }

    void Engine::safeReleaseObject(WeakPointer<CoreObject> object) {
        if(!Engine::isShuttingDown()) {
            Engine::instance()->objectManager.removeReference(object);
        }
    }

    void Engine::addOwner(WeakPointer<CoreObject> object) {
        this->objectManager.addReferenceOwner(object);
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
        this->objectManager.addReference(newScene, CoreObjectReferenceManager::OwnerType::Single);
        return newScene;
    }
    
    WeakPointer<Skeleton> Engine::createSkeleton(UInt32 boneCount, Bool multiOwner) {
        Skeleton * newSkeletonPtr = new(std::nothrow) Skeleton(boneCount);
        if (newSkeletonPtr == nullptr) {
            throw AllocationException("Engine::createSkeleton -> Could not allocate new skeleton.");
        }
        std::shared_ptr<Skeleton> spSkeleton = std::shared_ptr<Skeleton>(newSkeletonPtr);
        spSkeleton->init();
        CoreObjectReferenceManager::OwnerType ownerType = multiOwner ? CoreObjectReferenceManager::OwnerType::Multiple : 
                                                                       CoreObjectReferenceManager::OwnerType::Single;
        this->objectManager.addReference(spSkeleton, ownerType);
        return spSkeleton;
    }

    WeakPointer<VertexBoneMap> Engine::createVertexBoneMap(UInt32 vertexCount, UInt32 uVertexCount) {
        VertexBoneMap * newVertexBoneMapPtr = new(std::nothrow) VertexBoneMap(vertexCount, uVertexCount);
        if (newVertexBoneMapPtr == nullptr) {
            throw AllocationException("Engine::createVertexBoneMap -> Could not allocate new vertex bone map.");
        }
        std::shared_ptr<VertexBoneMap> newVertexBoneMap = std::shared_ptr<VertexBoneMap>(newVertexBoneMapPtr);
        Bool mapInitSuccess = newVertexBoneMap->init();
        if (!mapInitSuccess) {
            throw Exception("Engine::createVertexBoneMap -> Could not initialize vertex bone map.");
        }
        this->objectManager.addReference(newVertexBoneMap, CoreObjectReferenceManager::OwnerType::Single);
        return newVertexBoneMap;
    }

    WeakPointer<Mesh> Engine::createMesh(UInt32 size, UInt32 indexCount) {
        Mesh* newMeshPtr = new(std::nothrow) Mesh(size, indexCount);
        if (newMeshPtr == nullptr) {
            throw AllocationException("Engine::createMesh -> Unable to allocate new Mesh");
        }
        std::shared_ptr<Mesh> newMesh = std::shared_ptr<Mesh>(newMeshPtr);
        newMesh->init();
        this->objectManager.addReference(newMesh, CoreObjectReferenceManager::OwnerType::Single);
        return newMesh;
    }

    WeakPointer<Camera> Engine::createPerspectiveCamera(WeakPointer<Object3D> owner, Real fov, Real aspect, Real near, Real far) {
        std::shared_ptr<Camera> spCamera = std::shared_ptr<Camera>(Camera::createPerspectiveCamera(owner, fov, aspect, near, far));
        this->objectManager.addReference(spCamera, CoreObjectReferenceManager::OwnerType::Single);
        WeakPointer<Camera> wpCamera(spCamera);
        owner->addComponent(wpCamera);
        return wpCamera;
    }

    WeakPointer<Camera> Engine::createOrthographicCamera(WeakPointer<Object3D> owner, Real top, Real bottom, Real left, Real right, Real near, Real far) {
        std::shared_ptr<Camera> spCamera = std::shared_ptr<Camera>(Camera::createOrthographicCamera(owner, top, bottom, left, right, near, far));
        this->objectManager.addReference(spCamera, CoreObjectReferenceManager::OwnerType::Single);
        WeakPointer<Camera> wpCamera(spCamera);
        owner->addComponent(wpCamera);
        return wpCamera;
    }

    WeakPointer<Texture2D> Engine::createTexture2D(const TextureAttributes& attributes) {
        return this->graphics->createTexture2D(attributes);
    }

    WeakPointer<CubeTexture> Engine::createCubeTexture(const TextureAttributes& attributes) {
        return this->graphics->createCubeTexture(attributes);
    }

    WeakPointer<AttributeArrayGPUStorage> Engine::createGPUStorage(UInt32 size, UInt32 componentCount, AttributeType type, Bool normalize) {
        std::shared_ptr<AttributeArrayGPUStorage> spGPUStorage = this->graphics->createGPUStorage(size, componentCount, type, normalize);
        this->objectManager.addReference(spGPUStorage, CoreObjectReferenceManager::OwnerType::Single);
        return spGPUStorage;
    }

    WeakPointer<IndexBuffer> Engine::createIndexBuffer(UInt32 size) {
        std::shared_ptr<IndexBuffer> spIndexBufer = this->graphics->createIndexBuffer(size);
        this->objectManager.addReference(spIndexBufer, CoreObjectReferenceManager::OwnerType::Single);
        return spIndexBufer;
    }

    WeakPointer<ReflectionProbe> Engine::createReflectionProbe(WeakPointer<Object3D> owner) {
        ReflectionProbe* newReflectionProbePtr = new(std::nothrow) ReflectionProbe(owner);
        if (newReflectionProbePtr == nullptr) {
            throw AllocationException("Engine::createReflectionProbe -> Unable to allocate new ReflectionProbe");
        }
        std::shared_ptr<ReflectionProbe> spReflectionProbe = std::shared_ptr<ReflectionProbe>(newReflectionProbePtr);
        spReflectionProbe->init();
        this->objectManager.addReference(spReflectionProbe, CoreObjectReferenceManager::OwnerType::Single);
        WeakPointer<ReflectionProbe> wpReflectionProbe(spReflectionProbe);
        owner->addComponent(wpReflectionProbe);
        return spReflectionProbe;
    }

    WeakPointer<ParticleSystem> Engine::createParticleSystem(WeakPointer<Object3D> owner, UInt32 maxParticleCount) {
        ParticleSystem* newParticleSystemPtr = new(std::nothrow) ParticleSystem(owner, maxParticleCount);
        if (newParticleSystemPtr == nullptr) {
            throw AllocationException("Engine::createParticleSystem -> Unable to allocate new ParticleSystem");
        }
        std::shared_ptr<ParticleSystem> spParticleSystem = std::shared_ptr<ParticleSystem>(newParticleSystemPtr);
        this->objectManager.addReference(spParticleSystem, CoreObjectReferenceManager::OwnerType::Single);
        WeakPointer<ParticleSystem> wpParticleSystem(spParticleSystem);
        owner->addComponent(wpParticleSystem);
        this->particleSystemManager->addParticleSystem(wpParticleSystem);
        wpParticleSystem->addParticleStateOperator<BasicParticleStateOperator>();
        return spParticleSystem;
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