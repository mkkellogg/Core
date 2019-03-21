#include "ReflectionProbe.h"
#include "../geometry/Vector2.h"
#include "../image/TextureAttr.h"
#include "../render/Camera.h"
#include "../render/RenderTargetCube.h"
#include "../material/IrradianceRendererMaterial.h"
#include "../material/SpecularIBLRendererMaterial.h"
#include "../geometry/GeometryUtils.h"
#include "../scene/Object3D.h"

namespace Core {

    ReflectionProbe::ReflectionProbe(WeakPointer<Object3D> owner) : Object3DComponent(owner) {
        this->needsUpdate = false;
        this->skyboxOnly = true;
    }

    void ReflectionProbe::init() {
        Vector2u size(512, 512);

        Core::TextureAttributes colorAttributesScene;
        colorAttributesScene.Format = Core::TextureFormat::RGBA16F;
        colorAttributesScene.FilterMode = Core::TextureFilter::Linear;
        colorAttributesScene.MipLevel = 0;

        Core::TextureAttributes colorAttributesIrradiance;
        colorAttributesIrradiance.Format = Core::TextureFormat::RGBA16F;
        colorAttributesIrradiance.FilterMode = Core::TextureFilter::Linear;
        colorAttributesIrradiance.MipLevel = 0;

        Core::TextureAttributes colorAttributesSpecularIBL;
        colorAttributesSpecularIBL.Format = Core::TextureFormat::RGBA16F;
        colorAttributesSpecularIBL.FilterMode = Core::TextureFilter::TriLinear;
        colorAttributesSpecularIBL.MipLevel = 5;

        Core::TextureAttributes depthAttributes;
        depthAttributes.IsDepthTexture = true;

        this->sceneRenderTarget = Engine::instance()->getGraphicsSystem()->createRenderTargetCube(true, true, false, colorAttributesScene, depthAttributes, size);
        this->irradianceMap = Engine::instance()->getGraphicsSystem()->createRenderTargetCube(true, true, false, colorAttributesIrradiance, depthAttributes, size);
        this->specularIBLMap = Engine::instance()->getGraphicsSystem()->createRenderTargetCube(true, true, false, colorAttributesSpecularIBL, depthAttributes, size);

        this->renderCamera = Engine::instance()->createPerspectiveCamera(this->getOwner(), Core::Math::PI / 2.0f, 1.0, 0.1f, 100.0f);
        this->renderCamera->setRenderTarget(this->sceneRenderTarget);
        this->renderCamera->setAutoClearRenderBuffer(RenderBufferType::Color, true);
        this->renderCamera->setAutoClearRenderBuffer(RenderBufferType::Depth, true);
        this->renderCamera->setActive(false);
        this->renderCamera->setHDREnabled(false);

        Core::WeakPointer<Core::CubeTexture> sceneCubeTexture = Core::WeakPointer<Core::Texture>::dynamicPointerCast<Core::CubeTexture>(this->sceneRenderTarget->getColorTexture());

        this->irradianceRendererMaterial = Engine::instance()->createMaterial<IrradianceRendererMaterial>();
        this->irradianceRendererMaterial->setTexture(sceneCubeTexture);
        this->irradianceRendererMaterial->setFaceCullingEnabled(false);

        this->specularIBLRendererMaterial = Engine::instance()->createMaterial<SpecularIBLRendererMaterial>();
        this->specularIBLRendererMaterial->setTexture(sceneCubeTexture);
        this->specularIBLRendererMaterial->setFaceCullingEnabled(false);

        Color cubeColor(1.0f, 1.0f, 1.0f, 1.0f);
        WeakPointer<Mesh> cubeMesh = GeometryUtils::buildBoxMesh(1.0, 1.0, 1.0, cubeColor);
        this->skyboxCube = GeometryUtils::buildMeshContainer(cubeMesh, this->irradianceRendererMaterial, "irradianceCube");
    }

    void ReflectionProbe::setNeedsUpdate(Bool needsUpdate) {
        this->needsUpdate = needsUpdate;
    }

    Bool ReflectionProbe::getNeedsUpdate() {
        return this->needsUpdate;
    }

    WeakPointer<Camera> ReflectionProbe::getRenderCamera() {
        return this->renderCamera;
    }

    WeakPointer<Object3D> ReflectionProbe::getSkyboxObject() {
        return this->skyboxCube;
    }

    WeakPointer<RenderTargetCube> ReflectionProbe::getSceneRenderTarget() {
        return this->sceneRenderTarget;
    }

    WeakPointer<RenderTargetCube> ReflectionProbe::getIrradianceMap() {
        return this->irradianceMap;
    }

    WeakPointer<IrradianceRendererMaterial> ReflectionProbe::getIrradianceRendererMaterial() {
        return this->irradianceRendererMaterial;
    }

    void ReflectionProbe::setSkybox(Skybox& skybox) {
        this->renderCamera->setSkybox(skybox);
        this->renderCamera->setSkyboxEnabled(true);
    }

    void ReflectionProbe::setSkyboxOnly(Bool skyboxOnly) {
        this->skyboxOnly = skyboxOnly;
    }

    Bool ReflectionProbe::isSkyboxOnly() {
        return this->skyboxOnly;
    }
}
