#include "ParticleSystemAnimatedSpriteRenderer.h"
#include "../ParticleSystem.h"
#include "../material/ParticleStandardMaterial.h"
#include "../../material/Shader.h"
#include "../ParticleState.h"

namespace Core {

    ParticleSystemAnimatedSpriteRenderer::ParticleSystemAnimatedSpriteRenderer(WeakPointer<Object3D> owner):
        ParticleSystemRenderer(owner) {}

    ParticleSystemAnimatedSpriteRenderer::~ParticleSystemAnimatedSpriteRenderer() {
        if (this->material.isValid()) {
            Engine::safeReleaseObject(this->material);
        }
    }

    void ParticleSystemAnimatedSpriteRenderer::init() {
        this->material = Engine::instance()->createMaterial<ParticleStandardMaterial>();
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
        if (particleStates.getPositions()->getGPUStorage()) {
            particleStates.getPositions()->getGPUStorage()->sendToShader(worldPositionLocation);
        }
    }

    Bool ParticleSystemAnimatedSpriteRenderer::supportsRenderPath(RenderPath renderPath) {

    }

    UInt32 ParticleSystemAnimatedSpriteRenderer::getRenderQueueID() const {

    }

}
