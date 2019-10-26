#pragma once

#include <functional>
#include <memory>

#include "util/PersistentWeakPointer.h"
#include "scene/Object3D.h"
#include "asset/ModelLoader.h"
#include "geometry/Vector4.h"
#include "image/TextureAttr.h"
#include "material/Material.h"
#include "material/MaterialLibrary.h"
#include "render/RenderableContainer.h"
#include "render/Renderer.h"
#include "light/Light.h"
#include "light/PointLight.h"
#include "light/DirectionalLight.h"

namespace Core {

    // forward declarations
    class Graphics;
    class AnimationManager;
    class ImageLoader;
    class AssetLoader;
    class CubeTexture;
    class Texture2D;
    class BaseObjectRenderer;
    class Camera;
    class Scene;
    class ReflectionProbe;
    class Skeleton;
    class VertexBoneMap;
    class Mesh;

    class Engine final {
    public:
        typedef std::function<void()> LifecycleEventCallback;

        ~Engine();

        static WeakPointer<Engine> instance();
        static Bool isShuttingDown();

        void update();
        void render();

        void setRenderSize(UInt32 width, UInt32 height, Bool updateViewport = true);
        void setRenderSize(UInt32 width, UInt32 height, UInt32 hOffset, UInt32 vOffset, UInt32 viewPortWidth, UInt32 viewPortHeight);
        void setDefaultViewport(UInt32 hOffset, UInt32 vOffset, UInt32 viewPortWidth, UInt32 viewPortHeight);

        MaterialLibrary& getMaterialLibrary();
        ModelLoader& getModelLoader();

        WeakPointer<Graphics> getGraphicsSystem();
        WeakPointer<AnimationManager> getAnimationManager();

        void setActiveScene(WeakPointer<Scene> scene);
        WeakPointer<Scene> getActiveScene();
        WeakPointer<Scene> createScene();

        WeakPointer<Camera> createPerspectiveCamera(WeakPointer<Object3D> owner, Real fov, Real aspect, Real near, Real far);
        WeakPointer<Camera> createOrthographicCamera(WeakPointer<Object3D> owner, Real top, Real bottom, Real left, Real right, Real near, Real far);

        template <typename T>
        WeakPointer<typename std::enable_if<std::is_base_of<Light, T>::value, T>::type> createLight(WeakPointer<Object3D> owner) {
            std::shared_ptr<T> light = std::shared_ptr<T>(new T(owner));
            light->init();
            this->lights.push_back(light);
            WeakPointer<T> lightPtr = light;
            owner->addComponent(lightPtr);
            return lightPtr;
        }

        template <typename T>
        WeakPointer<typename std::enable_if<std::is_base_of<PointLight, T>::value, T>::type> 
        createPointLight(WeakPointer<Object3D> owner, Bool shadowsEnabled, UInt32 shadowMapSize, Real constantShadowBias, Real angularShadowBias) {
            std::shared_ptr<T> light = std::shared_ptr<T>(new T(owner, shadowsEnabled, shadowMapSize, constantShadowBias, angularShadowBias));
            light->init();
            this->lights.push_back(light);
            WeakPointer<T> lightPtr = light;
            owner->addComponent(lightPtr);
            return lightPtr;
        }

        template <typename T>
        WeakPointer<typename std::enable_if<std::is_base_of<DirectionalLight, T>::value, T>::type> 
        createDirectionalLight(WeakPointer<Object3D> owner, UInt32 cascadeCount, Bool shadowsEnabled, UInt32 shadowMapSize, Real constantShadowBias, Real angularShadowBias) {
            std::shared_ptr<T> light = std::shared_ptr<T>(new T(owner, cascadeCount, shadowsEnabled, shadowMapSize, constantShadowBias, angularShadowBias));
            light->init();
            this->lights.push_back(light);
            WeakPointer<T> lightPtr = light;
            owner->addComponent(lightPtr);
            return lightPtr;
        }

        template <typename T = Object3D>
        WeakPointer<typename std::enable_if<std::is_base_of<Object3D, T>::value, T>::type> createObject3D() {
            std::shared_ptr<T> objPtr = std::shared_ptr<T>(new T());
            WeakPointer<T> _temp = objPtr;
            objPtr->_self = _temp;
            objPtr->setName("GameObject");
            this->sceneObjects.push_back(objPtr);
            return objPtr;
        }

        WeakPointer<Skeleton> createSkeleton(UInt32 boneCount);
        WeakPointer<VertexBoneMap> createVertexBoneMap(UInt32 vertexCount, UInt32 uVertexCount);
        WeakPointer<Mesh> createMesh(UInt32 size, UInt32 indexCount);

        template <typename T, typename R>
        WeakPointer<typename std::enable_if<std::is_base_of<ObjectRenderer<R>, T>::value, T>::type> createRenderer(WeakPointer<Material> material,
                                                                                                                   WeakPointer<RenderableContainer<R>> owner) {
            std::shared_ptr<T> objectRenderer = std::shared_ptr<T>(new T(this->graphics, material, owner));
            owner->setRenderer(objectRenderer);
            objectRenderers.push_back(objectRenderer);
            return objectRenderer;
        }

        template <typename T>
        WeakPointer<typename std::enable_if<std::is_base_of<Material, T>::value, T>::type> createMaterial(Bool build = true) {
            std::shared_ptr<T> materialPtr = std::shared_ptr<T>(new T(this->graphics));
            if (build) {
                materialPtr->build();
            }
            this->materials.push_back(materialPtr);
            return materialPtr;
        }

        WeakPointer<Texture2D> createTexture2D(const TextureAttributes& attributes);
        WeakPointer<CubeTexture> createCubeTexture(const TextureAttributes& attributes);
        void destroyTexture2D(WeakPointer<Texture2D> texture);
        void destroyCubeTexture(WeakPointer<CubeTexture> texture);

        WeakPointer<ReflectionProbe> createReflectionProbe(WeakPointer<Object3D> owner);

        void setImageLoader(WeakPointer<ImageLoader> imageLoader);
        WeakPointer<ImageLoader> getImageLoader();
        void setAssetLoader(WeakPointer<AssetLoader> assetLoader);
        WeakPointer<AssetLoader> getAssetLoader();

        void setDefaultRenderTargetToCurrent();

        void onUpdate(LifecycleEventCallback func, Bool persistent = false);
        void onPreRender(LifecycleEventCallback func, Bool persistent = false);
        void onPostRender(LifecycleEventCallback func, Bool persistent = false);

    private:
        Engine();
        void init();
        void cleanup();
        void resolveRenderCallbacks(std::vector<LifecycleEventCallback>& oneTime, const std::vector<LifecycleEventCallback>& persistent);

        static std::shared_ptr<Engine> _instance;
        static Bool _shuttingDown;
        static void errorIfShuttingDown();
      
        std::vector<std::shared_ptr<Scene>> scenes;
        std::shared_ptr<Scene> activeScene;
        std::vector<std::shared_ptr<Camera>> cameras;
        std::vector<std::shared_ptr<Light>> lights;
        std::vector<std::shared_ptr<Object3D>> sceneObjects;
        std::vector<std::shared_ptr<Material>> materials;
        std::vector<std::shared_ptr<BaseObjectRenderer>> objectRenderers;
        std::vector<std::shared_ptr<Skeleton>> skeletons;
        std::vector<std::shared_ptr<VertexBoneMap>> vertexBoneMaps;
        std::vector<std::shared_ptr<Mesh>> meshes;
        std::vector<std::shared_ptr<ReflectionProbe>> reflectionProbes;

        PersistentWeakPointer<ImageLoader> imageLoader;
        PersistentWeakPointer<AssetLoader> assetLoader;
        std::vector<LifecycleEventCallback> updateCallbacks;
        std::vector<LifecycleEventCallback> preRenderCallbacks;
        std::vector<LifecycleEventCallback> postRenderCallbacks;
        std::vector<LifecycleEventCallback> persistentUpdateCallbacks;
        std::vector<LifecycleEventCallback> persistentPreRenderCallbacks;
        std::vector<LifecycleEventCallback> persistentPostRenderCallbacks;

        MaterialLibrary materialLibrary;
        ModelLoader modelLoader;
  
        std::shared_ptr<AnimationManager> animationManager;
        std::shared_ptr<Graphics> graphics;
        
    };
}
