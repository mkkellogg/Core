#include "TextureUtils.h"

#include "../Engine.h"
#include "../scene/Object3D.h"
#include "TextureAttr.h"
#include "Texture2D.h"
#include "../material/EquirectangularMaterial.h"
#include "../geometry/Vector2.h"
#include "../geometry/Mesh.h"
#include "../geometry/GeometryUtils.h"
#include "../render/Camera.h"
#include "../render/RenderTargetCube.h"

namespace Core {

    WeakPointer<CubeTexture> TextureUtils::loadFromEquirectangularImage(const std::string& filePath, Bool isHDR, float yRotation) {
        static WeakPointer<Object3D> cameraObj;
        static WeakPointer<Camera> renderCamera;
        static WeakPointer<EquirectangularMaterial> equirectangularMaterial;
        static WeakPointer<Object3D> cubeMeshObj;
        static Bool initialized = false;
        if (!initialized) {
            cameraObj = Engine::instance()->createObject3D<Object3D>();
            cameraObj->setName("EquirectangularRendererObject");
            renderCamera = Engine::instance()->createPerspectiveCamera(cameraObj, Core::Math::PI / 2.0f, 1.0, 0.1f, 100.0f);
            equirectangularMaterial =  Engine::instance()->createMaterial<EquirectangularMaterial>();
            Color cubeColor(1.0f, 1.0f, 1.0f, 1.0f);
            WeakPointer<Mesh> cubeMesh = GeometryUtils::buildBoxMesh(1.0, 1.0, 1.0, cubeColor);
            cubeMeshObj = Core::GeometryUtils::buildMeshContainerObject(cubeMesh, equirectangularMaterial, "EquirectangularCubeGeometry");
            initialized = true;
        }
        
        WeakPointer<Texture2D> equirectangularTexture;
        TextureAttributes equiTexAttributes;
        if (isHDR) {
            std::shared_ptr<Core::HDRImage> equiRectangularImage = Core::ImageLoader::loadImageHDR(filePath, true, false);
            equiTexAttributes.Format = Core::TextureFormat::RGBA16F;
            equirectangularTexture = Engine::instance()->createTexture2D(equiTexAttributes);
            equirectangularTexture->buildFromImage(equiRectangularImage);
        }
        else {
            std::shared_ptr<Core::StandardImage> equiRectangularImage = Core::ImageLoader::loadImageU(filePath, true, false);
            equiTexAttributes.Format = Core::TextureFormat::RGBA8;
            equirectangularTexture = Engine::instance()->createTexture2D(equiTexAttributes);
            equirectangularTexture->buildFromImage(equiRectangularImage);
        }
        
        equirectangularMaterial->setTexture(equirectangularTexture);
        equirectangularMaterial->setCullFace(Core::RenderState::CullFace::Front);

        Vector2u size(2048, 2048);
        TextureAttributes colorAttributes;
        colorAttributes.Format = Core::TextureFormat::RGBA16F;
        colorAttributes.FilterMode = Core::TextureFilter::Linear;
        Core::TextureAttributes depthAttributes;

        renderCamera->lookAt(Point3r(0.0f, 0.0f, -1.0f));
        WeakPointer<RenderTargetCube> renderTarget = Engine::instance()->getGraphicsSystem()->createRenderTargetCube(true, true, false, colorAttributes, depthAttributes, size);
        renderCamera->setRenderTarget(renderTarget);
        renderCamera->setAutoClearRenderBuffer(Core::RenderBufferType::Color, true);
        renderCamera->setAutoClearRenderBuffer(Core::RenderBufferType::Depth, true);
        renderCamera->setOverrideMaterial(equirectangularMaterial);
        cubeMeshObj->getTransform().lookAt(Core::Point3r(0.f, 0.0f, -1.0f));
        cubeMeshObj->getTransform().rotate(0.0f, 1.0f, 0.0f, yRotation);

        Engine::instance()->getGraphicsSystem()->getRenderer()->renderSceneBasic(cubeMeshObj, renderCamera, true);

        Graphics::safeReleaseObject(equirectangularTexture);
        WeakPointer<CubeTexture> cubeMap = WeakPointer<Texture>::dynamicPointerCast<CubeTexture>(renderTarget->getColorTexture());

        Graphics::safeReleaseObject(renderTarget);

        return cubeMap;
    }

}