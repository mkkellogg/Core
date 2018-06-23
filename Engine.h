#pragma once

#include <functional>
#include <memory>

#include "scene/Object3D.h"
#include "geometry/Mesh.h"
#include "asset/ModelLoader.h"
#include "geometry/Vector4.h"
#include "image/TextureAttr.h"
#include "material/Material.h"
#include "material/MaterialLibrary.h"
#include "render/RenderableContainer.h"
#include "render/Renderer.h"
#include "util/PersistentWeakPointer.h"

namespace Core {

    // forward declarations
    class ImageLoader;
    class AssetLoader;
    class CubeTexture;
    class Texture2D;
    class BaseObjectRenderer;
    class Camera;
    class Scene;

    class Engine {
    public:
        Engine();
        virtual ~Engine();

        void init();
        void update();

        void render();
        void setRenderSize(UInt32 width, UInt32 height, Bool updateViewport = true);
        void setRenderSize(UInt32 width, UInt32 height, UInt32 hOffset, UInt32 vOffset, UInt32 viewPortWidth, UInt32 viewPortHeight);
        void setViewport(UInt32 hOffset, UInt32 vOffset, UInt32 viewPortWidth, UInt32 viewPortHeight);

        MaterialLibrary& getMaterialLibrary();
        ModelLoader& getModelLoader();

        WeakPointer<Graphics> getGraphicsSystem();

        void setActiveScene(WeakPointer<Scene> scene);
        WeakPointer<Scene> getActiveScene();
        WeakPointer<Scene> createScene();

        WeakPointer<Camera> createCamera(WeakPointer<Object3D> owner);

        template <typename T = Object3D>
        WeakPointer<typename std::enable_if<std::is_base_of<Object3D, T>::value, T>::type> createObject3D() {
            std::shared_ptr<T> objPtr = std::shared_ptr<T>(new T());
            this->sceneObjects.push_back(objPtr);
            return objPtr;
        }

        WeakPointer<Mesh> createMesh(UInt32 size, Bool indexed);

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
            std::shared_ptr<T> materialPtr = std::shared_ptr<T>(new T(*this, this->graphics));
            if (build) {
                materialPtr->build();
            }
            this->materials.push_back(materialPtr);
            return materialPtr;
        }

        WeakPointer<Texture2D> createTexture2D(const TextureAttributes& attributes);
        WeakPointer<CubeTexture> createCubeTexture(const TextureAttributes& attributes);

        void setImageLoader(WeakPointer<ImageLoader> imageLoader);
        WeakPointer<ImageLoader> getImageLoader();
        void setAssetLoader(WeakPointer<AssetLoader> assetLoader);
        WeakPointer<AssetLoader> getAssetLoader();

        void onUpdate(std::function<void(Engine&)> func);

    private:
        std::shared_ptr<Graphics> graphics;

        std::vector<std::shared_ptr<Scene>> scenes;
        std::shared_ptr<Scene> activeScene;
        std::vector<std::shared_ptr<Camera>> cameras;
        std::vector<std::shared_ptr<Object3D>> sceneObjects;
        std::vector<std::shared_ptr<Material>> materials;
        std::vector<std::shared_ptr<Texture>> textures;
        std::vector<std::shared_ptr<BaseObjectRenderer>> objectRenderers;
        std::vector<std::shared_ptr<Mesh>> meshes;

        PersistentWeakPointer<ImageLoader> imageLoader;
        PersistentWeakPointer<AssetLoader> assetLoader;
        std::vector<std::function<void(Engine&)>> updateCallbacks;

        MaterialLibrary materialLibrary;
        ModelLoader modelLoader;

        void cleanup();
    };
}
