#pragma once

#include "../common/complextypes.h"
#include "../common/debug.h"
#include "../geometry/Vector2.h"
#include "../geometry/Vector4.h"
#include "../scene/Transform.h"
#include "../util/WeakPointer.h"

namespace Core {

    // forward declaration
    class Object3D;
    class Scene;
    class Camera;
    class Light;
    class ViewDescriptor;

    class Renderer {
    public:
        virtual ~Renderer();

        virtual Bool init();
        virtual void render(WeakPointer<Scene> scene);
        virtual void render(WeakPointer<Scene> scene, 
                            const ViewDescriptor& viewDescriptor, 
                            std::vector<WeakPointer<Object3D>>& objectList,
                            std::vector<WeakPointer<Light>>& lightList);
        void processScene(WeakPointer<Scene> scene, 
                          std::vector<WeakPointer<Object3D>>& outObjects, 
                          std::vector<WeakPointer<Camera>>& outCameras,
                          std::vector<WeakPointer<Light>>& outLights);

        void setRenderSize(UInt32 width, UInt32 height, Bool updateViewport = true);
        void setRenderSize(UInt32 width, UInt32 height, UInt32 hOffset, UInt32 vOffset, UInt32 viewPortWidth, UInt32 viewPortHeight);
        virtual void setViewport(UInt32 hOffset, UInt32 vOffset, UInt32 viewPortWidth, UInt32 viewPortHeight);
        Vector4u getViewport();

    private:
        void processSceneStep(WeakPointer<Object3D> object, 
                              const Matrix4x4& curTransform, 
                              std::vector<WeakPointer<Object3D>>& outObjects,
                              std::vector<WeakPointer<Camera>>& outCameras, 
                              std::vector<WeakPointer<Light>>& outLights);

    protected:
        Renderer();

        Vector2u renderSize;
        Vector4u viewport;
    };
}
