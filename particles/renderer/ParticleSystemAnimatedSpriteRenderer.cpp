#include "ParticleSystemAnimatedSpriteRenderer.h"
#include "../ParticleSystem.h"
#include "../material/ParticleStandardMaterial.h"
#include "../../material/Shader.h"
#include "../ParticleState.h"
#include "../../geometry/Vector2.h"
#include "../../geometry/AttributeArray.h"
#include "../../geometry/AttributeArrayGPUStorage.h"
#include "../../render/PrimitiveType.h"
#include "../../render/RenderState.h"
#include "../../render/EngineRenderQueue.h"

namespace Core {

    ParticleSystemAnimatedSpriteRenderer::ParticleSystemAnimatedSpriteRenderer(WeakPointer<Object3D> owner):
        ParticleSystemRenderer(owner) {}

    ParticleSystemAnimatedSpriteRenderer::~ParticleSystemAnimatedSpriteRenderer() {
        if (this->material.isValid()) {
            Engine::safeReleaseObject(this->material);
        }
    }

    Bool ParticleSystemAnimatedSpriteRenderer::init() {
        this->setCastShadows(false);
        this->material = Engine::instance()->createMaterial<ParticleStandardMaterial>();
        this->material->setBlendingMode(RenderState::BlendingMode::Custom);
        this->material->setSourceBlendingFactor(RenderState::BlendingFactor::SrcAlpha);
        this->material->setDestBlendingFactor(RenderState::BlendingFactor::OneMinusSrcAlpha);
        this->material->setRenderQueue(EngineRenderQueue::Transparent);
        return this->material.isValid();
    }

     void ParticleSystemAnimatedSpriteRenderer::setRenderState() {
        WeakPointer<Graphics> graphics = Engine::instance()->getGraphicsSystem();
        graphics->setColorWriteEnabled(this->material->getColorWriteEnabled());
        graphics->setRenderStyle(this->material->getRenderStyle());
        if (this->material->getBlendingMode() != RenderState::BlendingMode::None) {
            graphics->setBlendingEnabled(true);
            if (this->material->getBlendingMode() == RenderState::BlendingMode::Custom) {
                graphics->setBlendingEquation(this->material->getBlendingEquation());
            }
            graphics->setBlendingFactors(this->material->getSourceBlendingFactor(), this->material->getDestBlendingFactor());
        }
        else {
            graphics->setBlendingEnabled(false);
        }
        
        graphics->setDepthWriteEnabled(this->material->getDepthWriteEnabled());
        graphics->setDepthTestEnabled(this->material->getDepthTestEnabled());
        graphics->setDepthFunction(this->material->getDepthFunction());

        graphics->setFaceCullingEnabled(this->material->getFaceCullingEnabled());
        graphics->setCullFace(this->material->getCullFace());

        graphics->setStencilTestEnabled(this->material->getStencilTestEnabled());
        graphics->setStencilWriteMask(this->material->getStencilWriteMask());
        if (material->getStencilTestEnabled()) {
            graphics->setStencilFunction(this->material->getStencilComparisonFunction(), this->material->getStencilRef(), this->material->getStencilReadMask());
            graphics->setStencilOperation(this->material->getStencilFailActionStencil(), this->material->getStencilFailActionDepth(), this->material->getStencilAllPassAction());
        }
    }

    WeakPointer<ParticleStandardMaterial> ParticleSystemAnimatedSpriteRenderer::getMaterial() {
        return this->material;
    }

    Bool ParticleSystemAnimatedSpriteRenderer::forwardRender(const ViewDescriptor& viewDescriptor, const LightPack& lightPack, Bool matchPhysicalPropertiesWithLighting) {
        return true;
    }

    Bool ParticleSystemAnimatedSpriteRenderer::forwardRenderObject(const ViewDescriptor& viewDescriptor, WeakPointer<BaseRenderable> renderable, Bool isStatic,
                                                                   Int32 layer, const LightPack& lightPack, Bool matchPhysicalPropertiesWithLighting) {
        return true;
    }

    Bool ParticleSystemAnimatedSpriteRenderer::forwardRenderParticleSystem(const ViewDescriptor& viewDescriptor, WeakPointer<ParticleSystem> particleSystem, Bool isStatic,
                                                                           Int32 layer, const LightPack& lightPack, Bool matchPhysicalPropertiesWithLighting) {
        UInt32 maxActiveParticles = particleSystem->getMaximumActiveParticles();
        if (maxActiveParticles != this->renderAttributes.getParticleCount()) {
            this->renderAttributes.setParticleCount(maxActiveParticles);
        }

        WeakPointer<Shader> shader = this->material->getShader();
        WeakPointer<Graphics> graphics = Engine::instance()->getGraphicsSystem();
        graphics->activateShader(shader);

        this->setRenderState();

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
        WeakPointer<AttributeArray<Vector2rs>> sizes = particleStates.getSizes();
        WeakPointer<AttributeArrayGPUStorage> sizesGPUStorage = sizes->getGPUStorage();
        sizes->updateGPUStorageData();
        if (sizesGPUStorage.isValid()) {
            sizesGPUStorage->enableAndSendToActiveShader(sizeLocation);
        }

        Int32 rotationLocation = this->material->getRotationLocation();
        WeakPointer<ScalarAttributeArray<Real>> rotations = particleStates.getRotations();
        WeakPointer<AttributeArrayGPUStorage> rotationsGPUStorage = rotations->getGPUStorage();
        rotations->updateGPUStorageData();
        if (rotationsGPUStorage.isValid()) rotationsGPUStorage->enableAndSendToActiveShader(rotationLocation);

        Int32 sequenceElementLocation = this->material->getSequenceElementLocation();
        WeakPointer<AttributeArray<Vector4rs>> sequenceElements = particleStates.getSequenceElements();
        WeakPointer<AttributeArrayGPUStorage> sequenceElementsGPUStorage = sequenceElements->getGPUStorage();
        sequenceElements->updateGPUStorageData();
        if (sequenceElementsGPUStorage.isValid()) sequenceElementsGPUStorage->enableAndSendToActiveShader(sequenceElementLocation);

        Int32 colorLocation = this->material->getColorLocation();
        WeakPointer<AttributeArray<ColorS>> colors = particleStates.getColors();
        WeakPointer<AttributeArrayGPUStorage> colorsGPUStorage = colors->getGPUStorage();
        colors->updateGPUStorageData();
        if (colorsGPUStorage.isValid()) colorsGPUStorage->enableAndSendToActiveShader(colorLocation);

        this->material->sendCustomUniformsToShader();

        Engine::instance()->getGraphicsSystem()->drawBoundVertexBuffer(particleSystem->getActiveParticleCount(), PrimitiveType::Points);

        positionsGPUStorage->disable(worldPositionLocation);
        sizesGPUStorage->disable(sizeLocation);
        rotationsGPUStorage->disable(rotationLocation);

        return true;
    }

    Bool ParticleSystemAnimatedSpriteRenderer::supportsRenderPath(RenderPath renderPath) {
        return true;
    }

    UInt32 ParticleSystemAnimatedSpriteRenderer::getRenderQueueID() const {
        return this->material->getRenderQueueID();
    }

}
