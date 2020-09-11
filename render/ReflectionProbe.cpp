#include "ReflectionProbe.h"
#include "../geometry/Vector2.h"
#include "../geometry/GeometryUtils.h"
#include "../geometry/Mesh.h"
#include "../image/TextureAttr.h"
#include "../image/CubeTexture.h"
#include "../image/Texture2D.h"
#include "../render/Camera.h"
#include "../render/RenderTarget2D.h"
#include "../render/RenderTargetCube.h"
#include "../material/IrradianceRendererMaterial.h"
#include "../material/SpecularIBLPreFilteredRendererMaterial.h"
#include "../material/SpecularIBLBRDFRendererMaterial.h"
#include "../scene/Object3D.h"

namespace Core {

    ReflectionProbe::ReflectionProbe(WeakPointer<Object3D> owner) : Object3DComponent(owner) {
        this->needsFullUpdate = false;
        this->needsSpecularUpdate = false;
        this->skyboxOnly = true;
        this->renderWithPhysical = false;
    }

    ReflectionProbe::~ReflectionProbe() {
        if (this->skyboxCube.isValid()) Engine::safeReleaseObject(this->skyboxCube);
        if (this->specularIBLBRDFMapRenderTarget.isValid()) Graphics::safeReleaseObject(this->specularIBLBRDFMapRenderTarget);
        if (this->specularIBLPreFilteredMapRenderTarget.isValid()) Graphics::safeReleaseObject(this->specularIBLPreFilteredMapRenderTarget);
        if (this->irradianceMapRenderTarget.isValid()) Graphics::safeReleaseObject(this->irradianceMapRenderTarget);
    }

    void ReflectionProbe::init() {
        Vector2u size(512, 512);

        Core::TextureAttributes colorAttributesScene;
        colorAttributesScene.Format = Core::TextureFormat::RGBA16F;
        colorAttributesScene.FilterMode = Core::TextureFilter::Linear;
        colorAttributesScene.MipLevels = 0;

        Core::TextureAttributes colorAttributesIrradiance;
        colorAttributesIrradiance.Format = Core::TextureFormat::RGBA16F;
        colorAttributesIrradiance.FilterMode = Core::TextureFilter::Linear;
        colorAttributesIrradiance.MipLevels = 0;

        Core::TextureAttributes colorAttributesSpecularIBLPreFiltered;
        colorAttributesSpecularIBLPreFiltered.Format = Core::TextureFormat::RGBA16F;
        colorAttributesSpecularIBLPreFiltered.FilterMode = Core::TextureFilter::TriLinear;
        colorAttributesSpecularIBLPreFiltered.MipLevels = Core::Constants::MaxIBLLODLevels;

        Core::TextureAttributes colorAttributesSpecularIBLBRDF;
        colorAttributesSpecularIBLBRDF.Format = Core::TextureFormat::RG16F;
        colorAttributesSpecularIBLBRDF.FilterMode = Core::TextureFilter::Linear;
        colorAttributesSpecularIBLBRDF.MipLevels = 0;

        Core::TextureAttributes depthAttributes;
        depthAttributes.IsDepthTexture = true;

        this->sceneRenderTarget = Engine::instance()->getGraphicsSystem()->createRenderTargetCube(true, true, false, colorAttributesScene, depthAttributes, size);
        this->sceneRenderTarget->setMipLevel(0);
        this->irradianceMapRenderTarget = Engine::instance()->getGraphicsSystem()->createRenderTargetCube(true, true, false, colorAttributesIrradiance, depthAttributes, size);
        this->specularIBLPreFilteredMapRenderTarget = Engine::instance()->getGraphicsSystem()->createRenderTargetCube(true, true, false, colorAttributesSpecularIBLPreFiltered, depthAttributes, size);
        this->specularIBLBRDFMapRenderTarget = Engine::instance()->getGraphicsSystem()->createRenderTarget2D(true, true, false, colorAttributesSpecularIBLBRDF, depthAttributes, size);

        this->irradianceMap = WeakPointer<Texture>::dynamicPointerCast<CubeTexture>(this->irradianceMapRenderTarget->getColorTexture());
        this->specularIBLPreFilteredMap = WeakPointer<Texture>::dynamicPointerCast<CubeTexture>(this->specularIBLPreFilteredMapRenderTarget->getColorTexture());
        this->specularIBLBRDFMap = WeakPointer<Texture>::dynamicPointerCast<Texture2D>(this->specularIBLBRDFMapRenderTarget->getColorTexture());

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

        this->specularIBLPreFilteredRendererMaterial = Engine::instance()->createMaterial<SpecularIBLPreFilteredRendererMaterial>();
        this->specularIBLPreFilteredRendererMaterial->setTexture(sceneCubeTexture);
        this->specularIBLPreFilteredRendererMaterial->setFaceCullingEnabled(false);

        this->specularIBLBRDFRendererMaterial = Engine::instance()->createMaterial<SpecularIBLBRDFRendererMaterial>();
        this->specularIBLBRDFRendererMaterial->setFaceCullingEnabled(false);

        Color cubeColor(1.0f, 1.0f, 1.0f, 1.0f);
        WeakPointer<Mesh> cubeMesh = GeometryUtils::buildBoxMesh(1.0, 1.0, 1.0, cubeColor);
        this->skyboxCube = GeometryUtils::buildMeshContainer(cubeMesh, this->irradianceRendererMaterial, "irradianceCube");
    }

    void ReflectionProbe::setNeedsFullUpdate(Bool needsUpdate) {
        this->needsFullUpdate = needsUpdate;
    }

    Bool ReflectionProbe::getNeedsFullUpdate() {
        return this->needsFullUpdate;
    }

    void ReflectionProbe::setNeedsSpecularUpdate(Bool needsUpdate) {
        this->needsSpecularUpdate = needsUpdate;
    }

    Bool ReflectionProbe::getNeedsSpecularUpdate() {
        return this->needsSpecularUpdate;
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

    WeakPointer<RenderTargetCube> ReflectionProbe::getIrradianceMapRenderTarget() {
        return this->irradianceMapRenderTarget;
    }

    WeakPointer<CubeTexture> ReflectionProbe::getIrradianceMap() {
        return this->irradianceMap;
    }

    WeakPointer<RenderTargetCube> ReflectionProbe::getSpecularIBLPreFilteredMapRenderTarget() {
        return this->specularIBLPreFilteredMapRenderTarget;
    }

    WeakPointer<CubeTexture> ReflectionProbe::getSpecularIBLPreFilteredMap() {
        return this->specularIBLPreFilteredMap;
    }

    WeakPointer<RenderTarget2D> ReflectionProbe::getSpecularIBLBRDFMapRenderTarget() {
        return this->specularIBLBRDFMapRenderTarget;
    }

    WeakPointer<Texture2D> ReflectionProbe::getSpecularIBLBRDFMap() {
        return this->specularIBLBRDFMap;
    }

    WeakPointer<IrradianceRendererMaterial> ReflectionProbe::getIrradianceRendererMaterial() {
        return this->irradianceRendererMaterial;
    }

    WeakPointer<SpecularIBLPreFilteredRendererMaterial> ReflectionProbe::getSpecularIBLPreFilteredRendererMaterial() {
        return this->specularIBLPreFilteredRendererMaterial;
    }

    WeakPointer<SpecularIBLBRDFRendererMaterial> ReflectionProbe::getSpecularIBLBRDFRendererMaterial() {
        return this->specularIBLBRDFRendererMaterial;
    }

    void ReflectionProbe::setSkybox(Skybox& skybox) {
        this->renderCamera->setSkybox(skybox);
        this->renderCamera->setSkyboxEnabled(true);
    }

    void ReflectionProbe::setSkyboxOnly(Bool skyboxOnly) {
        this->skyboxOnly = skyboxOnly;
    }

    void ReflectionProbe::setRenderWithPhysical(Bool renderWithPhysical) {
        this->renderWithPhysical = renderWithPhysical;
    }

    Bool ReflectionProbe::getRenderWithPhysical() {
        return renderWithPhysical;
    }

    Bool ReflectionProbe::isSkyboxOnly() {
        return this->skyboxOnly;
    }
}
