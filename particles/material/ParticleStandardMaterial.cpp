#include "ParticleStandardMaterial.h"
#include "../../material/Shader.h"
#include "../../Engine.h"
#include "../../Graphics.h"
#include "../../material/ShaderManager.h"
#include "../../image/Texture.h"
#include "../../image/Texture2D.h"

namespace Core {

    ParticleStandardMaterial::ParticleStandardMaterial() {
        this->worldPositionLocation = -1;
        this->sizeLocation = -1;
        this->rotationLocation = -1;
        this->projectionMatrixLocation = -1;
        this->viewMatrixLocation = -1;
    }

    Bool ParticleStandardMaterial::build() {
        WeakPointer<Graphics> graphics = Engine::instance()->getGraphicsSystem();
        ShaderManager& shaderManager = graphics->getShaderManager();
        this->shader = shaderManager.getShader("ParticleStandard");
        graphics->activateShader(this->shader);
        this->bindShaderVarLocations();
        this->setSkinningEnabled(false);
        this->setDepthWriteEnabled(false);
        return true;
    }

    Int32 ParticleStandardMaterial::getShaderLocation(StandardAttribute attribute, UInt32 offset) {
        return -1;
    }

    Int32 ParticleStandardMaterial::getShaderLocation(StandardUniform uniform, UInt32 offset) {
        switch (uniform) {
            case StandardUniform::ProjectionMatrix:
                return this->projectionMatrixLocation;
            case StandardUniform::ViewMatrix:
                return this->viewMatrixLocation;
            default:
                return -1;
        }
    }

    Int32 ParticleStandardMaterial::getWorldPositionLocation() {
        return this->worldPositionLocation;
    }

    Int32 ParticleStandardMaterial::getSizeLocation() {
        return this->sizeLocation;
    }

    Int32 ParticleStandardMaterial::getRotationLocation() {
        return this->rotationLocation;
    }

    void ParticleStandardMaterial::copyTo(WeakPointer<Material> target) {
        WeakPointer<ParticleStandardMaterial> particleMaterial = WeakPointer<Material>::dynamicPointerCast<ParticleStandardMaterial>(target);
        if (particleMaterial.isValid()) {
            Material::copyTo(target);
            particleMaterial->projectionMatrixLocation = this->projectionMatrixLocation;
            particleMaterial->viewMatrixLocation = this->viewMatrixLocation;
            particleMaterial->worldPositionLocation = this->worldPositionLocation;
            particleMaterial->sizeLocation = this->sizeLocation;
            particleMaterial->rotationLocation = this->rotationLocation;
        } else {
            throw InvalidArgumentException("ParticleStandardMaterial::copyTo() -> 'target must be same material.");
        }
    }

    WeakPointer<Material> ParticleStandardMaterial::clone() {
        WeakPointer<ParticleStandardMaterial> newMaterial = Engine::instance()->createMaterial<ParticleStandardMaterial>();
        this->copyTo(newMaterial);
        return newMaterial;
    }

    void ParticleStandardMaterial::bindShaderVarLocations() {
        this->projectionMatrixLocation = this->shader->getUniformLocation(StandardUniform::ProjectionMatrix);
        this->viewMatrixLocation = this->shader->getUniformLocation(StandardUniform::ViewMatrix);
        this->worldPositionLocation = this->shader->getAttributeLocation("worldPosition");
        this->sizeLocation = this->shader->getAttributeLocation("size");
        this->rotationLocation = this->shader->getAttributeLocation("rotation");
    }
}
