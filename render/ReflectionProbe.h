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
    class RenderTarget2D;
    class IrradianceRendererMaterial;
    class SpecularIBLPreFilteredRendererMaterial;
    class SpecularIBLBRDFRendererMaterial;
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
        WeakPointer<RenderTargetCube> getSpecularIBLPreFilteredMap();
        WeakPointer<RenderTarget2D> getSpecularIBLBRDFMap();
        WeakPointer<IrradianceRendererMaterial> getIrradianceRendererMaterial();
        WeakPointer<SpecularIBLPreFilteredRendererMaterial> getSpecularIBLPreFilteredRendererMaterial();
        WeakPointer<SpecularIBLBRDFRendererMaterial> getSpecularIBLBRDFRendererMaterial();

    private:
        Bool needsUpdate;
        Bool skyboxOnly;
        PersistentWeakPointer<RenderTargetCube> sceneRenderTarget;
        PersistentWeakPointer<RenderTargetCube> irradianceMap;
        PersistentWeakPointer<RenderTargetCube> specularIBLPreFilteredMap;
        PersistentWeakPointer<RenderTarget2D> specularIBLBRDFMap;
        PersistentWeakPointer<Object3D> renderCameraObject;
        PersistentWeakPointer<Camera> renderCamera;
        PersistentWeakPointer<IrradianceRendererMaterial> irradianceRendererMaterial;
        PersistentWeakPointer<SpecularIBLPreFilteredRendererMaterial> specularIBLPreFilteredRendererMaterial;
        PersistentWeakPointer<SpecularIBLBRDFRendererMaterial> specularIBLBRDFRendererMaterial;
        PersistentWeakPointer<Object3D> skyboxCube;
    };

}
