#include "ReflectionProbe.h"
#include "../geometry/Vector2.h"
#include "../image/TextureAttr.h"
#include "../render/Camera.h"
#include "../render/RenderTargetCube.h"
#include "../material/IrradianceRendererMaterial.h"
#include "../geometry/GeometryUtils.h"
#include "../scene/Object3D.h"

namespace Core {

    ReflectionProbe::ReflectionProbe(WeakPointer<Object3D> owner) : Object3DComponent(owner) {
        needsUpdate = false;
    }

    void ReflectionProbe::init() {
        Vector2u size(512, 512);
        Core::TextureAttributes colorAttributesScene;
        colorAttributesScene.Format = Core::TextureFormat::RGBA16F;
        colorAttributesScene.FilterMode = Core::TextureFilter::Linear;
        Core::TextureAttributes colorAttributesIrradiance;
        colorAttributesIrradiance.Format = Core::TextureFormat::RGBA16F;
        colorAttributesIrradiance.FilterMode = Core::TextureFilter::Linear;
        Core::TextureAttributes depthAttributes;
        depthAttributes.IsDepthTexture = true;
        this->sceneRenderTarget = Engine::instance()->getGraphicsSystem()->createRenderTargetCube(true, true, false, colorAttributesScene, depthAttributes, size);
        this->irradianceMap = Engine::instance()->getGraphicsSystem()->createRenderTargetCube(true, true, false, colorAttributesIrradiance, depthAttributes, size);

        this->renderCamera = Engine::instance()->createPerspectiveCamera(this->getOwner(), Core::Math::PI / 2.0f, 1.0, 0.1f, 100.0f);
        this->renderCamera->setRenderTarget(this->sceneRenderTarget);
        this->renderCamera->setAutoClearRenderBuffer(RenderBufferType::Color, true);
        this->renderCamera->setAutoClearRenderBuffer(RenderBufferType::Depth, true);
        this->renderCamera->setActive(false);
        this->renderCamera->setHDREnabled(false);

        this->irradianceRendererMaterial = Engine::instance()->createMaterial<IrradianceRendererMaterial>();
        Core::WeakPointer<Core::CubeTexture> sceneCubeTexture = Core::WeakPointer<Core::Texture>::dynamicPointerCast<Core::CubeTexture>(this->sceneRenderTarget->getColorTexture());
        this->irradianceRendererMaterial->setTexture(sceneCubeTexture);
        this->irradianceRendererMaterial->setFaceCullingEnabled(false);

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
}
