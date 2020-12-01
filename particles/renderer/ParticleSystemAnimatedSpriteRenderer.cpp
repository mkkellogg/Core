#include "ParticleSystemAnimatedSpriteRenderer.h"
#include "../ParticleSystem.h"
#include "../material/ParticleStandardMaterial.h"
#include "../../material/Shader.h"
#include "../ParticleState.h"
#include "../../geometry/AttributeArray.h"
#include "../../geometry/AttributeArrayGPUStorage.h"
#include "../../render/PrimitiveType.h"

namespace Core {

    ParticleSystemAnimatedSpriteRenderer::ParticleSystemAnimatedSpriteRenderer(WeakPointer<Object3D> owner):
        ParticleSystemRenderer(owner) {}

    ParticleSystemAnimatedSpriteRenderer::~ParticleSystemAnimatedSpriteRenderer() {
        if (this->material.isValid()) {
            Engine::safeReleaseObject(this->material);
        }
    }

    Bool ParticleSystemAnimatedSpriteRenderer::init() {
        this->material = Engine::instance()->createMaterial<ParticleStandardMaterial>();
        return this->material.isValid();
    }

    Bool ParticleSystemAnimatedSpriteRenderer::forwardRender(const ViewDescriptor& viewDescriptor, const LightPack& lightPack, Bool matchPhysicalPropertiesWithLighting) {

    }

    Bool ParticleSystemAnimatedSpriteRenderer::forwardRenderObject(const ViewDescriptor& viewDescriptor, WeakPointer<BaseRenderable> renderable, Bool isStatic,
                                                                   const LightPack& lightPack, Bool matchPhysicalPropertiesWithLighting) {

    }

    Bool ParticleSystemAnimatedSpriteRenderer::forwardRenderParticleSystem(const ViewDescriptor& viewDescriptor, WeakPointer<ParticleSystem> particleSystem, Bool isStatic,
                                                                           const LightPack& lightPack, Bool matchPhysicalPropertiesWithLighting) {
        UInt32 maxActiveParticles = particleSystem->getMaximumActiveParticles();
        if (maxActiveParticles != this->renderAttributes.getParticleCount()) {
            this->renderAttributes.setParticleCount(maxActiveParticles);
        }

        WeakPointer<Shader> shader = this->material->getShader();
        WeakPointer<Graphics> graphics = Engine::instance()->getGraphicsSystem();
        graphics->activateShader(shader);

        Int32 projectionLoc = this->material->getShaderLocation(StandardUniform::ProjectionMatrix);
        Int32 viewMatrixLoc = this->material->getShaderLocation(StandardUniform::ViewMatrix);
        if (projectionLoc >= 0) shader->setUniformMatrix4(projectionLoc, viewDescriptor.projectionMatrix);
        if (viewMatrixLoc >= 0) shader->setUniformMatrix4(viewMatrixLoc, viewDescriptor.inverseCameraTransformation);

        ParticleStateAttributeArray& particleStates = particleSystem->getParticleStates();

        Int32 worldPositionLocation = this->material->getWorldPositionLocation();
        WeakPointer<AttributeArray<Point3rs>> positions = particleStates.getPositions();
        WeakPointer<AttributeArrayGPUStorage> positionsGPUStorage = positions->getGPUStorage();
        positions->updateGPUStorageData();
        if (positionsGPUStorage.isValid()) positionsGPUStorage->enableAndSendToActiveShader(worldPositionLocation);

        Int32 sizeLocation = this->material->getSizeLocation();
        WeakPointer<ScalarAttributeArray<Real>> radiuses = particleStates.getRotations();
        WeakPointer<AttributeArrayGPUStorage> radiusesGPUStorage = radiuses->getGPUStorage();
        radiuses->updateGPUStorageData();
        if (radiusesGPUStorage.isValid()) radiusesGPUStorage->enableAndSendToActiveShader(sizeLocation);

        Int32 rotationLocation = this->material->getRotationLocation();
        WeakPointer<ScalarAttributeArray<Real>> rotations = particleStates.getRotations();
        WeakPointer<AttributeArrayGPUStorage> rotationsGPUStorage = rotations->getGPUStorage();
        rotations->updateGPUStorageData();
        if (rotationsGPUStorage.isValid()) rotationsGPUStorage->enableAndSendToActiveShader(rotationLocation);

        Engine::instance()->getGraphicsSystem()->drawBoundVertexBuffer(particleSystem->getActiveParticleCount(), PrimitiveType::Points);

        positionsGPUStorage->disable(worldPositionLocation);
        radiusesGPUStorage->disable(sizeLocation);
        rotationsGPUStorage->disable(rotationLocation);
    }

    Bool ParticleSystemAnimatedSpriteRenderer::supportsRenderPath(RenderPath renderPath) {

    }

    UInt32 ParticleSystemAnimatedSpriteRenderer::getRenderQueueID() const {

    }

}
