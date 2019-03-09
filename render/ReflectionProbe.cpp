#include "ReflectionProbe.h"
#include "../geometry/Vector2.h"
#include "../image/TextureAttr.h"
#include "../render/Camera.h"
#include "../render/RenderTargetCube.h"
#include "../material/IrridianceRendererMaterial.h"
#include "../geometry/GeometryUtils.h"
#include "../scene/Object3D.h"

namespace Core {

    ReflectionProbe::ReflectionProbe(WeakPointer<Object3D> owner) : Object3DComponent(owner) {
        needsUpdate = false;
    }

    void ReflectionProbe::init() {
        Vector2u size(512, 512);
        Core::TextureAttributes colorAttributes;
        colorAttributes.Format = Core::TextureFormat::RGBA8;
        colorAttributes.FilterMode = Core::TextureFilter::Linear;
        Core::TextureAttributes depthAttributes;
        this->sceneRenderTarget = Engine::instance()->getGraphicsSystem()->createRenderTargetCube(true, true, false, colorAttributes, depthAttributes, size);
        this->irridianceMap = Engine::instance()->getGraphicsSystem()->createRenderTargetCube(true, true, false, colorAttributes, depthAttributes, size);

        this->renderCamera = Engine::instance()->createPerspectiveCamera(this->getOwner(), Core::Math::PI / 2.0f, 1.0, 0.1f, 100.0f);
        this->renderCamera->setRenderTarget(this->sceneRenderTarget);
        this->renderCamera->setAutoClearRenderBuffer(RenderBufferType::Color, true);
        this->renderCamera->setAutoClearRenderBuffer(RenderBufferType::Depth, true);
        this->renderCamera->setActive(false);

        this->irridianceRendererMaterial = Engine::instance()->createMaterial<IrridianceRendererMaterial>();
        Core::WeakPointer<Core::CubeTexture> sceneCubeTexture = Core::WeakPointer<Core::Texture>::dynamicPointerCast<Core::CubeTexture>(this->sceneRenderTarget->getColorTexture());
        this->irridianceRendererMaterial->setTexture(sceneCubeTexture);
        this->irridianceRendererMaterial->setFaceCullingEnabled(false);

        Color cubeColor(1.0f, 1.0f, 1.0f, 1.0f);
        WeakPointer<Mesh> cubeMesh = GeometryUtils::buildBoxMesh(1.0, 1.0, 1.0, cubeColor);
        this->skyboxCube = GeometryUtils::buildMeshContainer(cubeMesh, this->irridianceRendererMaterial, "irridianceCube");
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

    WeakPointer<RenderTargetCube> ReflectionProbe::getIrridianceMap() {
        return this->irridianceMap;
    }

    WeakPointer<IrridianceRendererMaterial> ReflectionProbe::getIrridianceRendererMaterial() {
        return this->irridianceRendererMaterial;
    }

    void ReflectionProbe::setSkybox(Skybox& skybox) {
        this->renderCamera->setSkybox(skybox);
    }
}
