#include "ParticleStandardMaterial.h"
#include "../../material/Shader.h"
#include "../../Engine.h"
#include "../../Graphics.h"
#include "../../material/ShaderManager.h"
#include "../../image/Texture.h"
#include "../../image/Texture2D.h"

namespace Core {

    ParticleStandardMaterial::ParticleStandardMaterial() {
        this->interpolateAtlasFramesLocation = -1;
        this->worldPositionLocation = -1;
        this->sizeLocation = -1;
        this->rotationLocation = -1;
        this->sequenceElementLocation = -1;
        this->colorLocation = -1;
        this->projectionMatrixLocation = -1;
        this->viewMatrixLocation = -1;

        this->interpolateAtlasFrames = false;
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

    void ParticleStandardMaterial::sendCustomUniformsToShader() {
        WeakPointer<Texture2D> atlasTexture = this->atlas.getTexture();
        if (atlasTexture.isValid() && this->atlasTextureLocation >= 0) {
            this->shader->setTexture2D(0, atlasTextureLocation, atlasTexture->getTextureID());
            if (this->atlasTileArayLocation >= 0) {
                UInt32 atlasTileArrayCount = this->atlas.getTileArrayCount();
                for (UInt32 i = 0; i < atlasTileArrayCount; i++) {
                    Atlas::TileArrayDescriptor& tileArray = this->atlas.getTileArray(i);
                    this->shader->setUniform4f(atlasTileArayLocation[i], tileArray.x, tileArray.y, tileArray.width, tileArray.height);
                }
            }
        }
        if (this->uvOffsetLocation >= 0) {
            this->shader->setUniform2f(this->uvOffsetLocation, this->uvOffset.x, this->uvOffset.y);
        }
        if (this->interpolateAtlasFramesLocation >= 0) {
            this->shader->setUniform1i(this->interpolateAtlasFramesLocation, this->interpolateAtlasFrames ? 1 : 0);
        }
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

    Int32 ParticleStandardMaterial::getSequenceElementLocation() {
        return this->sequenceElementLocation;
    }

    Int32 ParticleStandardMaterial::getColorLocation() {
        return this->colorLocation;;
    }

    void ParticleStandardMaterial::copyTo(WeakPointer<Material> target) {
        WeakPointer<ParticleStandardMaterial> particleMaterial = WeakPointer<Material>::dynamicPointerCast<ParticleStandardMaterial>(target);
        if (particleMaterial.isValid()) {
            Material::copyTo(target);
            particleMaterial->atlas = this->atlas;
            particleMaterial->interpolateAtlasFramesLocation = this->interpolateAtlasFramesLocation;
            particleMaterial->projectionMatrixLocation = this->projectionMatrixLocation;
            particleMaterial->viewMatrixLocation = this->viewMatrixLocation;
            particleMaterial->atlasTextureLocation = this->atlasTextureLocation;
            particleMaterial->uvOffsetLocation = this->uvOffsetLocation;
            for (UInt32 i = 0; i < Constants::MaxAtlasTileArrays; i++) {
                particleMaterial->atlasTileArayLocation[i] = this->atlasTileArayLocation[i];
            }
            particleMaterial->worldPositionLocation = this->worldPositionLocation;
            particleMaterial->sizeLocation = this->sizeLocation;
            particleMaterial->rotationLocation = this->rotationLocation;
            particleMaterial->sequenceElementLocation = this->sequenceElementLocation;
            particleMaterial->colorLocation = this->colorLocation;
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
        this->interpolateAtlasFramesLocation = this->shader->getUniformLocation("interpolateAtlasFrames");
        this->projectionMatrixLocation = this->shader->getUniformLocation(StandardUniform::ProjectionMatrix);
        this->viewMatrixLocation = this->shader->getUniformLocation(StandardUniform::ViewMatrix);
        this->atlasTextureLocation = this->shader->getUniformLocation("atlasTexture");
        this->uvOffsetLocation = this->shader->getUniformLocation("uvOffset");
        for (UInt32 i = 0; i < Constants::MaxAtlasTileArrays; i++) {
            this->atlasTileArayLocation[i] = this->shader->getUniformLocation("atlasTileArray", i);
        }
        this->worldPositionLocation = this->shader->getAttributeLocation("worldPosition");
        this->sizeLocation = this->shader->getAttributeLocation("size");
        this->rotationLocation = this->shader->getAttributeLocation("rotation");
        this->sequenceElementLocation = this->shader->getAttributeLocation("sequenceElement");
        this->colorLocation = this->shader->getAttributeLocation("color");
    }

    const Atlas& ParticleStandardMaterial::getAtlas() const {
        return this->atlas;
    }

    void ParticleStandardMaterial::setAtlas(const Atlas& atlas) {
        this->atlas = atlas;
    }

    void ParticleStandardMaterial::setUVOffset(Real x, Real y) {
        this->uvOffset.set(x, y);
    }

    void ParticleStandardMaterial::setInterpolateAtlasFrames(Bool interpolateAtlasFrames) {
        this->interpolateAtlasFrames = interpolateAtlasFrames;
    }
}
