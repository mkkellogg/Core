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
    class IrridianceRendererMaterial;

    class ReflectionProbe : public Object3DComponent {
    public:
        ReflectionProbe(WeakPointer<Object3D> owner);
        void init();
        void setNeedsUpdate(Bool needsUpdate);
        Bool getNeedsUpdate();
        WeakPointer<Camera> getRenderCamera();
        WeakPointer<Object3D> getSkyboxObject();
        WeakPointer<RenderTargetCube> getSceneRenderTarget();
        WeakPointer<RenderTargetCube> getIrridianceMap();
        WeakPointer<IrridianceRendererMaterial> getIrridianceRendererMaterial();

    private:
        Bool needsUpdate;
        WeakPointer<RenderTargetCube> sceneRenderTarget;
        WeakPointer<RenderTargetCube> irridianceMap;
        WeakPointer<Object3D> renderCameraObject;
        WeakPointer<Camera> renderCamera;
        WeakPointer<IrridianceRendererMaterial> irridianceRendererMaterial;
        WeakPointer<Object3D> skyboxCube;
    };

}
