#pragma once

#include <vector>

#include "../common/complextypes.h"
#include "../common/debug.h"
#include "../base/CoreObject.h"
#include "RenderBuffer.h"
#include "RenderState.h"
#include "RenderQueueManager.h"
#include "RenderList.h"
#include "../geometry/Vector2.h"
#include "../geometry/Vector4.h"
#include "../scene/Transform.h"
#include "../util/WeakPointer.h"
#include "../light/LightType.h"
#include "../light/LightPack.h"
#include "../base/BitMask.h"
#include "DepthOutputOverride.h"
#include "CubeFace.h"

namespace Core {

    // forward declaration
    class Object3D;
    class Scene;
    class Camera;
    class Light;
    class DirectionalLight;
    class PointLight;
    class AmbientLight;
    class AmbientIBLLight;
    class ViewDescriptor;
    class DepthOnlyMaterial;
    class NormalsMaterial;
    class PositionsMaterial;
    class SSAOMaterial;
    class SSAOBlurMaterial;
    class PositionsAndNormalsMaterial;
    class DistanceOnlyMaterial;
    class TonemapMaterial;
    class Material;
    class RenderTarget;
    class RenderTarget2D;
    class ReflectionProbe;
    class Skybox;
    class Texture2D;

    class Renderer : public CoreObject {
    public:
        virtual ~Renderer();
        virtual Bool init();
        void renderScene(WeakPointer<Scene> scene, WeakPointer<Material> overrideMaterial = WeakPointer<Material>::nullPtr());
        void renderScene(WeakPointer<Object3D> rootObject, WeakPointer<Material> overrideMaterial = WeakPointer<Material>::nullPtr());
        void renderSceneBasic(WeakPointer<Object3D> rootObject, WeakPointer<Camera> camera, Bool matchPhysicalPropertiesWithLighting);
        void renderObjectDirect(WeakPointer<Object3D> object, WeakPointer<Camera> camera, Bool matchPhysicalPropertiesWithLighting);
        void renderObjectDirect(WeakPointer<Object3D> object, WeakPointer<Camera> camera, const LightPack& lightPack,
                                Bool matchPhysicalPropertiesWithLighting);
        WeakPointer<Texture2D> getSSAOTexture();
    protected:
        Renderer();
        void renderForCamera(WeakPointer<Camera> camera, std::vector<WeakPointer<Object3D>>& objects, 
                             Bool matchPhysicalPropertiesWithLighting);
        void renderForCamera(WeakPointer<Camera> camera, std::vector<WeakPointer<Object3D>>& objects, 
                             const LightPack& lightPack, Bool matchPhysicalPropertiesWithLighting);
        void renderForStandardCamera(WeakPointer<Camera> camera, std::vector<WeakPointer<Object3D>>& objects, 
                                     const LightPack& lightPack, Bool matchPhysicalPropertiesWithLighting,
                                     WeakPointer<Texture2D> ssaoMap = WeakPointer<Texture2D>::nullPtr());
        void renderForCubeCamera(WeakPointer<Camera> camera, std::vector<WeakPointer<Object3D>>& objects, 
                                 const LightPack& lightPack, Bool matchPhysicalPropertiesWithLighting);
        void renderForViewDescriptor(ViewDescriptor& viewDescriptor, std::vector<WeakPointer<Object3D>>& objectList, 
                                     const LightPack& lightPack, Bool matchPhysicalPropertiesWithLighting);
        void renderForViewDescriptor(ViewDescriptor& viewDescriptor, RenderQueueManager& renderQueueManager, 
                                     const LightPack& lightPack, Bool matchPhysicalPropertiesWithLighting);
        void renderSkybox(ViewDescriptor& viewDescriptor);
        void renderObjectDirect(WeakPointer<Object3D> object, ViewDescriptor& viewDescriptor, const LightPack& lightPack,
                                Bool matchPhysicalPropertiesWithLighting);
        void renderDirectionalLightShadowMaps(const std::vector<WeakPointer<DirectionalLight>>& lightList,
                                              std::vector<WeakPointer<Object3D>>& objects, WeakPointer<Camera> renderCamera);
        void renderPointLightShadowMaps(const std::vector<WeakPointer<PointLight>>& lightList, std::vector<WeakPointer<Object3D>>& objects);
        void setViewportAndMipLevelForRenderTarget(WeakPointer<RenderTarget> renderTarget, Int16 cubeFace);
        void clearActiveRenderTarget(ViewDescriptor& viewDescriptor);
        void getViewDescriptorForCubeCamera(WeakPointer<Camera> camera, CubeFace cubeFace, ViewDescriptor& outDescriptor);
        void getViewDescriptorsForCubeCamera(WeakPointer<Camera> camera, ViewDescriptor& descForward, ViewDescriptor& descBackward,
                                             ViewDescriptor& descUp, ViewDescriptor& descDown, ViewDescriptor& descLeft, ViewDescriptor& descRight);
        void getViewDescriptorForCamera(WeakPointer<Camera> camera, ViewDescriptor& viewDescriptor);
        void getViewDescriptorTransformations(const Matrix4x4& worldMatrix, const Matrix4x4& projectionMatrix,
                               IntMask clearBuffers, ViewDescriptor& viewDescriptor);
        void collectSceneObjectsAndComputeTransforms(WeakPointer<Scene> scene, std::vector<WeakPointer<Object3D>>& outObjects);
        void collectSceneObjectsAndComputeTransforms(WeakPointer<Object3D> object, std::vector<WeakPointer<Object3D>>& outObjects);
        void collectSceneObjectsAndComputeTransforms(WeakPointer<Object3D> object, std::vector<WeakPointer<Object3D>>& outObjects, const Matrix4x4& curTransform);
        void collectSceneObjectComponents(std::vector<WeakPointer<Object3D>>& sceneObjects, std::vector<WeakPointer<Camera>>& cameraList,
                                          std::vector<WeakPointer<ReflectionProbe>>& reflectionProbeList, std::vector<WeakPointer<Light>>& nonIBLLightList,
                                          std::vector<WeakPointer<DirectionalLight>>& directionalLightList, std::vector<WeakPointer<PointLight>>& pointLightList,
                                          std::vector<WeakPointer<AmbientLight>>& ambientLightList, std::vector<WeakPointer<AmbientIBLLight>>& ambientIBLLightList,
                                          std::vector<WeakPointer<Light>>& lightList);
        void renderReflectionProbes(std::vector<WeakPointer<ReflectionProbe>>& reflectionProbeList, std::vector<WeakPointer<Object3D>> staticObjects,
                                    const LightPack& lightPack, const LightPack& nonIBLLightPack);
        void renderReflectionProbe(WeakPointer<ReflectionProbe> reflectionProbe, Bool specularOnly,
                                   std::vector<WeakPointer<Object3D>>& renderObjects, const LightPack& lightPack);
        void renderSSAO(WeakPointer<Camera> camera, std::vector<WeakPointer<Object3D>>& objects);
        void renderDepthAndNormals(ViewDescriptor& viewDescriptor, std::vector<WeakPointer<Object3D>>& objects);
        void renderPositionsAndNormals(ViewDescriptor& viewDescriptor, std::vector<WeakPointer<Object3D>>& objects);
        void initializeSSAO();

        void sortObjectsIntoRenderQueues(std::vector<WeakPointer<Object3D>>& objects, RenderQueueManager& renderQueueManager, Int32 overrideRenderQueueID=-1);
        void buildRenderListFromObjects(std::vector<WeakPointer<Object3D>>& objects, RenderList& renderList);

        static Bool isShadowCastingCapableLight(WeakPointer<Light> light);
        static Bool compareLights (WeakPointer<Light> a, WeakPointer<Light> b);

        PersistentWeakPointer<DepthOnlyMaterial> depthMaterial;
        PersistentWeakPointer<NormalsMaterial> normalsMaterial;
        PersistentWeakPointer<PositionsMaterial> positionsMaterial;
        PersistentWeakPointer<PositionsAndNormalsMaterial> positionsNormalsMaterial;
        PersistentWeakPointer<DistanceOnlyMaterial> distanceMaterial;
        PersistentWeakPointer<Object3D> reflectionProbeObject;
        PersistentWeakPointer<TonemapMaterial> tonemapMaterial;
        PersistentWeakPointer<Camera> perspectiveShadowMapCamera;
        PersistentWeakPointer<Object3D> perspectiveShadowMapCameraObject;
        PersistentWeakPointer<Camera> orthoShadowMapCamera;
        PersistentWeakPointer<Object3D> orthoShadowMapCameraObject;

        PersistentWeakPointer<RenderTarget2D> depthNormalsRenderTarget;
        PersistentWeakPointer<RenderTarget2D> depthPositionsRenderTarget;
        PersistentWeakPointer<RenderTarget2D> positionsNormalsRenderTarget;

        PersistentWeakPointer<RenderTarget2D> ssaoRenderTarget;
        PersistentWeakPointer<SSAOMaterial> ssaoMaterial;
        PersistentWeakPointer<RenderTarget2D> ssaoBlurRenderTarget;
        PersistentWeakPointer<Texture2D> ssaoBlurMap;
        PersistentWeakPointer<SSAOBlurMaterial> ssaoBlurMaterial;
        WeakPointer<Texture2D> ssaoNoise;
        std::vector<Vector3r> ssaoKernel;
    };
}
