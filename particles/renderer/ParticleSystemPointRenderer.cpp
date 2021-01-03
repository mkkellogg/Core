#include "ParticleSystemPointRenderer.h"
#include "../ParticleSystem.h"
#include "../ParticleState.h"
#include "../../geometry/GeometryUtils.h"
#include "../../render/MeshContainer.h"
#include "../../render/MeshRenderer.h"
#include "../../geometry/Mesh.h"
#include "../../material/BasicMaterial.h"

namespace Core {

    ParticleSystemPointRenderer::ParticleSystemPointRenderer(WeakPointer<Object3D> owner):
        ParticleSystemRenderer(owner) {}

    ParticleSystemPointRenderer::~ParticleSystemPointRenderer() {}

    Bool ParticleSystemPointRenderer::init() {
        this->setCastShadows(false);
        return true;
    }

    Bool ParticleSystemPointRenderer::forwardRender(const ViewDescriptor& viewDescriptor, const LightPack& lightPack, Bool matchPhysicalPropertiesWithLighting) {
        return true;
    }

    Bool ParticleSystemPointRenderer::forwardRenderObject(const ViewDescriptor& viewDescriptor, WeakPointer<BaseRenderable> renderable, Bool isStatic,
                                                          Int32 layer, const LightPack& lightPack, Bool matchPhysicalPropertiesWithLighting) {
        return true;
    }

    Bool ParticleSystemPointRenderer::forwardRenderParticleSystem(const ViewDescriptor& viewDescriptor, WeakPointer<ParticleSystem> particleSystem, Bool isStatic,
                                                                  Int32 layer, const LightPack& lightPack, Bool matchPhysicalPropertiesWithLighting) {
        this->updatePointMeshCount(particleSystem);
        UInt32 maxActiveParticles = particleSystem->getMaximumActiveParticles();
        if (maxActiveParticles != this->renderAttributes.getParticleCount()) {
            this->renderAttributes.setParticleCount(maxActiveParticles);
        }
        Bool setParent = false;
        for (UInt32 i = 0; i < particleSystem->getActiveParticleCount(); i++) {
            ParticleStatePtr& particleState = particleSystem->getParticleStatePtr(i);
            WeakPointer<Object3D> meshRoot = this->pointMeshRoots[i];
            meshRoot->setActive(true);
            WeakPointer<Object3D> meshRootParent = meshRoot->getParent();
            if (particleSystem->getSimulateInWorldSpace()) {
                if(!meshRootParent.isValid() || meshRootParent.get() != this->worldRenderRoot.get()) {
                    this->worldRenderRoot->addChild(meshRoot);
                }
            } else {
                if(!meshRootParent.isValid() || meshRootParent.get() != this->localRenderRoot.get()) {
                    this->localRenderRoot->addChild(meshRoot);
                }
            }
            setParent = true;
            const Point3rs& pos = *particleState.position;
            meshRoot->getTransform().setLocalPosition(pos.x, pos.y, pos.z);
        }
        for (UInt32 i = particleSystem->getActiveParticleCount(); i < maxActiveParticles; i++) {
            WeakPointer<Object3D> meshRoot = this->pointMeshRoots[i];
            meshRoot->setActive(false);
        }
        this->worldRenderRoot->getTransform().getWorldMatrix().setIdentity();
        return true;
    }

    Bool ParticleSystemPointRenderer::supportsRenderPath(RenderPath renderPath) {
        return true;
    }

    UInt32 ParticleSystemPointRenderer::getRenderQueueID() const {
        return 0;
    }

    void ParticleSystemPointRenderer::updatePointMeshCount(WeakPointer<ParticleSystem> particleSystem) {
        static Color sphereColor(1.0f, 1.0f, 1.0f, 1.0f);
        static WeakPointer<BasicMaterial> basicMaterial = Engine::instance()->createMaterial<BasicMaterial>();
        if (!this->localRenderRoot.isValid()) {
            this->localRenderRoot = Engine::instance()->createObject3D<Object3D>();
            this->owner->addChild(this->localRenderRoot);
            this->localRenderRoot->getTransform().setLocalPosition(0.0f, 0.0f, 0.0f);

            this->worldRenderRoot = Engine::instance()->createObject3D<Object3D>();
            this->owner->addChild(this->worldRenderRoot);
            this->worldRenderRoot->getTransform().setMatrixAutoUpdate(false);
        }
        for (UInt32 i = this->pointMeshes.size(); i < particleSystem->getMaximumActiveParticles(); i++) {
            WeakPointer<Mesh> sphereMesh = GeometryUtils::buildSphereMesh(0.15f, 32, sphereColor);
            this->pointMeshes.push_back(sphereMesh);

            WeakPointer<Object3D> meshRoot = Engine::instance()->createObject3D<Object3D>();
            //this->owner->addChild(this->localRenderRoot);
            //this->localRenderRoot->getTransform().setLocalPosition(0.0f, 0.0f, 0.0f);

            WeakPointer<MeshContainer> meshContainer = Engine::instance()->createRenderableContainer<MeshContainer, Mesh>(meshRoot);
            WeakPointer<MeshRenderer> localRenderer = Engine::instance()->createRenderer<MeshRenderer, Mesh>(basicMaterial, meshRoot);
            meshContainer->addRenderable(sphereMesh);
            this->pointMeshRoots.push_back(meshRoot);
        }
    }
}
