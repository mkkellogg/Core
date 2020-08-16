#include "Graphics.h"
#include "scene/Scene.h"
#include "scene/Object3D.h"
#include "image/Texture2D.h"
#include "image/CubeTexture.h"
#include "material/Shader.h"
#include "material/Material.h"
#include "render/Renderer.h"
#include "render/MeshRenderer.h"
#include "render/ObjectRenderer.h"
#include "render/BaseObjectRenderer.h"
#include "render/MeshContainer.h"
#include "render/Camera.h"
#include "render/RenderTarget.h"
#include "render/RenderTarget2D.h"
#include "geometry/AttributeArrayGPUStorage.h"
#include "geometry/IndexBuffer.h"
#include "geometry/Mesh.h"
#include "geometry/GeometryUtils.h"

namespace Core {

    Graphics::Graphics(): sharedRenderState(false) {
    }

    Graphics::~Graphics() {
    }

    void Graphics::init() {
        TextureAttributes texAttributes;
        texAttributes.FilterMode = TextureFilter::Point;
        texAttributes.MipLevels = 0;
        texAttributes.WrapMode = TextureWrap::Mirror;
        texAttributes.Format = TextureFormat::RGBA8;
        this->placeHolderTexture2D = this->createTexture2D(texAttributes);
        this->placeHolderCubeTexture = this->createCubeTexture(texAttributes);
    }

    void Graphics::safeReleaseObject(WeakPointer<CoreObject> object) {
        if(!Engine::isShuttingDown()) {
            Engine::instance()->getGraphicsSystem()->objectManager.removeReference(object);
        }
    }

    WeakPointer<Texture2D> Graphics::getPlaceHolderTexture2D() {
        return this->placeHolderTexture2D;
    }
    
    WeakPointer<CubeTexture> Graphics::getPlaceHolderCubeTexture() {
        return this->placeHolderCubeTexture;
    }

    void Graphics::setSharedRenderState(Bool shared) {
        this->sharedRenderState = shared;
    }

    void Graphics::setRenderSize(UInt32 width, UInt32 height, Bool updateViewport) {
        this->updateDefaultRenderTargetSize(Vector2u(width, height));
        if (updateViewport) {
            this->setDefaultViewport(0, 0, width, height);
        }
    }

    void Graphics::setRenderSize(UInt32 width, UInt32 height, UInt32 hOffset, UInt32 vOffset, UInt32 viewPortWidth, UInt32 viewPortHeight) {
        this->updateDefaultRenderTargetSize(Vector2u(width, height));
        this->updateDefaultRenderTargetViewport(Vector4u(hOffset, vOffset, viewPortWidth, viewPortHeight));
    }

    void Graphics::setDefaultViewport(UInt32 hOffset, UInt32 vOffset, UInt32 viewPortWidth, UInt32 viewPortHeight) {
        this->updateDefaultRenderTargetViewport(Vector4u(hOffset, vOffset, viewPortWidth, viewPortHeight));
    }

    void Graphics::blit(WeakPointer<RenderTarget> source, WeakPointer<RenderTarget> destination, Int16 cubeFace, WeakPointer<Material> material, Bool includeDepth) {
        UInt32 samplerSlot = 0;
        Int32 texture0Loc = material->getShaderLocation(StandardUniform::Texture0);
        if (texture0Loc >= 0) {
           material->getShader()->setTexture2D(samplerSlot, texture0Loc, source->getColorTexture()->getTextureID());
           samplerSlot++;
        }

        Int32 depthTextureLoc = material->getShaderLocation(StandardUniform::DepthTexture);
        if (depthTextureLoc >= 0 && source->isDepthBufferTexture()) {
            material->getShader()->setTexture2D(samplerSlot, depthTextureLoc, source->getDepthTexture()->getTextureID());
            samplerSlot++;
        }

        this->renderFullScreenQuad(destination, cubeFace, material);
        if (includeDepth) {
            this->lowLevelBlit(source, destination, -1, false, true);
        }
    }

    void Graphics::renderFullScreenQuad(WeakPointer<RenderTarget> destination, Int16 cubeFace, WeakPointer<Material> material) {
        static Bool initialized = false;
        static WeakPointer<Mesh> fullScreenQuadMesh;
        static WeakPointer<MeshContainer> fullScreenQuadObject;
        static WeakPointer<Mesh> fullScreenCubeMesh;
        static WeakPointer<MeshContainer> fullScreenCubeObject;
        static WeakPointer<Object3D> renderCameraObject;
        static WeakPointer<Camera> renderCamera;
        if (!initialized) {
            fullScreenQuadMesh = GeometryUtils::createGridMesh(2.0f, 2.0f, 2, 2, 1.0f, 1.0f);
            fullScreenQuadObject = GeometryUtils::buildMeshContainer(fullScreenQuadMesh, material, "");
            renderCameraObject = Engine::instance()->createObject3D();
            renderCameraObject->getTransform().translate(0.0f, 0.0f, 4.0f);
            renderCameraObject->getTransform().updateWorldMatrix();
            renderCamera = Engine::instance()->createOrthographicCamera(renderCameraObject, 1.0f, -1.0f, -1.0f, 1.0f, 0.1f, 10.0f);
            initialized = true;
        }

        Bool depthTestEnabled = material->getDepthTestEnabled();
        Bool faceCullingEnabled = material->getFaceCullingEnabled();
        material->setDepthTestEnabled(false);
        material->setFaceCullingEnabled(false);
        renderCamera->setRenderTarget(destination);
        
        WeakPointer<RenderTarget> currentRenderTarget = this->getCurrentRenderTarget();
        Vector4u currentViewport = currentRenderTarget->getViewport();

        UInt32 targetMipLevel = destination->getMipLevel();
        this->activateRenderTarget(destination);
        if (cubeFace >= 0) {
            this->activateCubeRenderTargetSide((CubeTextureSide)cubeFace, targetMipLevel);
        }
        else {
            this->activateRenderTarget2DMipLevel(targetMipLevel);
        }
        
        Vector4u mipLevelScaledViewport = destination->getViewportForMipLevel(targetMipLevel);
        this->setViewport(mipLevelScaledViewport.x, mipLevelScaledViewport.y, mipLevelScaledViewport.z, mipLevelScaledViewport.w);

        WeakPointer<Material> savedOverideMaterial = renderCamera->getOverrideMaterial();
        renderCamera->setOverrideMaterial(material);
        this->getRenderer()->renderObjectDirect(fullScreenQuadObject, renderCamera);
        renderCamera->setOverrideMaterial(savedOverideMaterial);

        material->setFaceCullingEnabled(faceCullingEnabled);
        material->setDepthTestEnabled(depthTestEnabled);

        this->activateRenderTarget(currentRenderTarget);
        this->setViewport(currentViewport.x, currentViewport.y, currentViewport.z, currentViewport.w);
    }

    void Graphics::addCoreObjectReference(std::shared_ptr<CoreObject> object, CoreObjectReferenceManager::OwnerType ownerType) {
        this->objectManager.addReference(object, ownerType);
    }

}