#pragma once

#include "../common/complextypes.h"
#include "../common/debug.h"
#include "../base/CoreObject.h"
#include "RenderBuffer.h"
#include "RenderState.h"
#include "RenderQueueManager.h"
#include "../geometry/Vector2.h"
#include "../geometry/Vector4.h"
#include "../scene/Transform.h"
#include "../util/WeakPointer.h"
#include "../light/LightType.h"
#include "../base/BitMask.h"

namespace Core {

    // forward declaration
    class Object3D;
    class Scene;
    class Camera;
    class Light;
    class ViewDescriptor;
    class DepthOnlyMaterial;
    class DistanceOnlyMaterial;
    class TonemapMaterial;
    class Material;
    class RenderTarget;
    class RenderTarget2D;
    class ReflectionProbe;
    class Skybox;

    class Renderer : public CoreObject {
    public:
        virtual ~Renderer();
        virtual Bool init();
        void renderScene(WeakPointer<Scene> scene, WeakPointer<Material> overrideMaterial = WeakPointer<Material>::nullPtr());
        void renderScene(WeakPointer<Object3D> rootObject, WeakPointer<Material> overrideMaterial = WeakPointer<Material>::nullPtr());
        void renderObjectBasic(WeakPointer<Object3D> rootObject, WeakPointer<Camera> camera,
                               WeakPointer<Material> overrideMaterial = WeakPointer<Material>::nullPtr(),
                               Bool matchPhysicalPropertiesWithLighting = true);
        void renderObjectDirect(WeakPointer<Object3D> object, WeakPointer<Camera> camera,
                                WeakPointer<Material> overrideMaterial = WeakPointer<Material>::nullPtr(),
                                Bool matchPhysicalPropertiesWithLighting = true);
        void renderObjectDirect(WeakPointer<Object3D> object, WeakPointer<Camera> camera, std::vector<WeakPointer<Light>>& lightList,
                                WeakPointer<Material> overrideMaterial = WeakPointer<Material>::nullPtr(),
                                Bool matchPhysicalPropertiesWithLighting = true);

    protected:
        Renderer();
        void renderStandard(WeakPointer<Camera> camera, std::vector<WeakPointer<Object3D>>& objects, 
                            std::vector<WeakPointer<Light>>& lights, WeakPointer<Material> overrideMaterial,
                            Bool matchPhysicalPropertiesWithLighting);
        void renderCube(WeakPointer<Camera> camera, std::vector<WeakPointer<Object3D>>& objects, 
                        std::vector<WeakPointer<Light>>& lights, WeakPointer<Material> overrideMaterial,
                        Bool matchPhysicalPropertiesWithLighting);
        void render(WeakPointer<Camera> camera, std::vector<WeakPointer<Object3D>>& objects, 
                    WeakPointer<Material> overrideMaterial,
                    Bool matchPhysicalPropertiesWithLighting);
        void render(WeakPointer<Camera> camera, std::vector<WeakPointer<Object3D>>& objects, 
                    std::vector<WeakPointer<Light>>& lights, WeakPointer<Material> overrideMaterial,
                    Bool matchPhysicalPropertiesWithLighting);
        void render(ViewDescriptor& viewDescriptor, std::vector<WeakPointer<Object3D>>& objectList, 
                    std::vector<WeakPointer<Light>>& lightList,
                    Bool matchPhysicalPropertiesWithLighting);
        void setViewportAndMipLevelForRenderTarget(WeakPointer<RenderTarget> renderTarget, Int16 cubeFace);
        void clearActiveRenderTarget(ViewDescriptor& viewDescriptor);
        void renderSkybox(ViewDescriptor& viewDescriptor);
        void renderObjectDirect(WeakPointer<Object3D> object, ViewDescriptor& viewDescriptor, std::vector<WeakPointer<Light>>& lightList,
                                Bool matchPhysicalPropertiesWithLighting);
        void renderShadowMaps(std::vector<WeakPointer<Light>>& lights, LightType lightType, 
                              std::vector<WeakPointer<Object3D>>& objects, WeakPointer<Camera> renderCamera = WeakPointer<Camera>());
        void getViewDescriptorForCamera(WeakPointer<Camera> camera, ViewDescriptor& viewDescriptor);
        void getViewDescriptorTransformations(const Matrix4x4& worldMatrix, const Matrix4x4& projectionMatrix,
                               IntMask clearBuffers, ViewDescriptor& viewDescriptor);
        void collectSceneObjectsAndComputeTransforms(WeakPointer<Scene> scene, std::vector<WeakPointer<Object3D>>& outObjects);
        void collectSceneObjectsAndComputeTransforms(WeakPointer<Object3D> object, std::vector<WeakPointer<Object3D>>& outObjects);
        void collectSceneObjectsAndComputeTransforms(WeakPointer<Object3D> object, std::vector<WeakPointer<Object3D>>& outObjects, const Matrix4x4& curTransform);
        void collectSceneObjectComponents(std::vector<WeakPointer<Object3D>>& sceneObjects, std::vector<WeakPointer<Camera>>& cameraList,
                                          std::vector<WeakPointer<ReflectionProbe>>& reflectionProbeList, std::vector<WeakPointer<Light>>& nonIBLLightList,
                                          std::vector<WeakPointer<Light>>& lightList);
        void renderReflectionProbes(std::vector<WeakPointer<ReflectionProbe>>& reflectionProbeList, std::vector<WeakPointer<Object3D>> staticObjects,
                                    std::vector<WeakPointer<Light>>& nonIBLLightList, std::vector<WeakPointer<Light>>& lightList);
        void renderReflectionProbe(WeakPointer<ReflectionProbe> reflectionProbe, Bool specularOnly,
                                   std::vector<WeakPointer<Object3D>>& renderObjects, std::vector<WeakPointer<Light>>& renderLights);
        
        void sortObjectsIntoRenderQueues(std::vector<WeakPointer<Object3D>>& objects, RenderQueueManager& renderQueueManager, ViewDescriptor& viewDescriptor);

        static Bool isShadowCastingCapableLight(WeakPointer<Light> light);
        static Bool compareLights (WeakPointer<Light> a, WeakPointer<Light> b);

        RenderQueueManager renderQueueManager;

        PersistentWeakPointer<DepthOnlyMaterial> depthMaterial;
        PersistentWeakPointer<DistanceOnlyMaterial> distanceMaterial;
        PersistentWeakPointer<Object3D> reflectionProbeObject;
        PersistentWeakPointer<TonemapMaterial> tonemapMaterial;

        PersistentWeakPointer<Camera> perspectiveShadowMapCamera;
        PersistentWeakPointer<Object3D> perspectiveShadowMapCameraObject;
        PersistentWeakPointer<Camera> orthoShadowMapCamera;
        PersistentWeakPointer<Object3D> orthoShadowMapCameraObject;
    };
}
