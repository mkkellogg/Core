#pragma once

#include <memory>
#include <vector>

#include "../Engine.h"
#include "../util/WeakPointer.h"
#include "../scene/Object3DComponent.h"

namespace Core {

    // forward declarations
    class Object3D;
    class Camera;
    class Light;
    class RenderTargetCube;
    class IrradianceRendererMaterial;
    class Skybox;

    class ReflectionProbe : public Object3DComponent {
    public:
        ReflectionProbe(WeakPointer<Object3D> owner);
        void init();
        void setNeedsUpdate(Bool needsUpdate);
        Bool getNeedsUpdate();
        void setSkybox(Skybox& skybox);
        void setSkyboxOnly(Bool skyboxOnly);
        Bool isSkyboxOnly();
        WeakPointer<Camera> getRenderCamera();
        WeakPointer<Object3D> getSkyboxObject();
        WeakPointer<RenderTargetCube> getSceneRenderTarget();
        WeakPointer<RenderTargetCube> getIrradianceMap();
        WeakPointer<IrradianceRendererMaterial> getIrradianceRendererMaterial();

    private:
        Bool needsUpdate;
        Bool skyboxOnly;
        PersistentWeakPointer<RenderTargetCube> sceneRenderTarget;
        PersistentWeakPointer<RenderTargetCube> irradianceMap;
        PersistentWeakPointer<Object3D> renderCameraObject;
        PersistentWeakPointer<Camera> renderCamera;
        PersistentWeakPointer<IrradianceRendererMaterial> irradianceRendererMaterial;
        PersistentWeakPointer<Object3D> skyboxCube;
    };

}
