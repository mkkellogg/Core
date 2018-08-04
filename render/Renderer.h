#pragma once

#include "../common/complextypes.h"
#include "../common/debug.h"
#include "../geometry/Vector2.h"
#include "../geometry/Vector4.h"
#include "../scene/Transform.h"
#include "../util/WeakPointer.h"
#include "../light/LightType.h"

namespace Core {

    // forward declaration
    class Object3D;
    class Scene;
    class Camera;
    class Light;
    class ViewDescriptor;
    class DepthOnlyMaterial;
    class DistanceOnlyMaterial;
    class Material;

    class Renderer {
    public:
        virtual ~Renderer();

        virtual Bool init();
        void render(WeakPointer<Scene> scene);
        void render(WeakPointer<Camera> camera, std::vector<WeakPointer<Object3D>>& objects, 
                    std::vector<WeakPointer<Light>>& lights, WeakPointer<Material> overrideMaterial = WeakPointer<Material>::nullPtr());
    protected:
        Renderer();
        void renderStandard(WeakPointer<Camera> camera, std::vector<WeakPointer<Object3D>>& objects, 
                            std::vector<WeakPointer<Light>>& lights, WeakPointer<Material> overrideMaterial);
        void renderCube(WeakPointer<Camera> camera, std::vector<WeakPointer<Object3D>>& objects, 
                        std::vector<WeakPointer<Light>>& lights, WeakPointer<Material> overrideMaterial);
        void render(const ViewDescriptor& viewDescriptor, std::vector<WeakPointer<Object3D>>& objectList, 
                    std::vector<WeakPointer<Light>>& lightList);
        void renderShadowMaps(std::vector<WeakPointer<Light>>& lights, LightType lightType, 
                              std::vector<WeakPointer<Object3D>>& objects, WeakPointer<Camera> renderCamera = WeakPointer<Camera>());
        void getViewDescriptorForCamera(WeakPointer<Camera> camera, ViewDescriptor& viewDescriptor);
        void getViewDescriptorForCamera(const Matrix4x4& worldMatrix, const Matrix4x4& projectionMatrix, ViewDescriptor& viewDescriptor);
        void processScene(WeakPointer<Scene> scene, std::vector<WeakPointer<Object3D>>& outObjects,
                          std::vector<WeakPointer<Camera>>& outCameras, std::vector<WeakPointer<Light>>& outLights);
        void processSceneStep(WeakPointer<Object3D> object, const Matrix4x4& curTransform, std::vector<WeakPointer<Object3D>>& outObjects,
                              std::vector<WeakPointer<Camera>>& outCameras, std::vector<WeakPointer<Light>>& outLights);
        static Bool isShadowCastingCapableLight(WeakPointer<Light> light);

        WeakPointer<DepthOnlyMaterial> depthMaterial;
        WeakPointer<DistanceOnlyMaterial> distanceMaterial;
    };
}
