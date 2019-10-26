#include "BasicColoredMaterial.h"
#include "../material/Shader.h"
#include "../util/WeakPointer.h"
#include "StandardAttributes.h"
#include "StandardUniforms.h"
#include "../Engine.h"
#include "../material/ShaderManager.h"

namespace Core {

    BasicColoredMaterial::BasicColoredMaterial(WeakPointer<Graphics> graphics) : Material(graphics) {
        this->zOffset = 0.0f;
    }

    Bool BasicColoredMaterial::build() {
        WeakPointer<Graphics> graphics = Engine::instance()->getGraphicsSystem();
        ShaderManager& shaderDirectory = graphics->getShaderManager();
        const std::string& vertexSrc = shaderDirectory.getShader(ShaderType::Vertex, "BasicColored");
        const std::string& fragmentSrc = shaderDirectory.getShader(ShaderType::Fragment, "BasicColored");
        Bool ready = this->buildFromSource(vertexSrc, fragmentSrc);
        if (!ready) {
            return false;
        }

        this->bindShaderVarLocations();
        this->setSkinningEnabled(true);
        return true;
    }

    Int32 BasicColoredMaterial::getShaderLocation(StandardAttribute attribute, UInt32 offset) {
        switch (attribute) {
            case StandardAttribute::Position:
                return this->positionLocation;
            case StandardAttribute::BoneIndex:
                return this->boneIndexLocation;
            case StandardAttribute::BoneWeight:
                return this->boneWeightLocation;
            default:
                return -1;
        }
    }

    Int32 BasicColoredMaterial::getShaderLocation(StandardUniform uniform, UInt32 offset) {
        switch (uniform) {
            case StandardUniform::ProjectionMatrix:
                return this->projectionMatrixLocation;
            case StandardUniform::ViewMatrix:
                return this->viewMatrixLocation;
            case StandardUniform::ModelMatrix:
                return this->modelMatrixLocation;
            case StandardUniform::SkinningEnabled:
                return this->skinningEnabledLocation;
            case StandardUniform::Bones:
                return this->bonesLocation[offset];
            default:
                return -1;
        }
    }

    void BasicColoredMaterial::sendCustomUniformsToShader() {
        this->shader->setUniform4f(this->colorLocation, this->color.r, this->color.g, this->color.b, this->color.a);
        this->shader->setUniform1f(this->zOffsetLocation, this->zOffset);
    }

    WeakPointer<Material> BasicColoredMaterial::clone() {
        WeakPointer<BasicColoredMaterial> newMaterial = Engine::instance()->createMaterial<BasicColoredMaterial>(false);
        this->copyTo(newMaterial);
        newMaterial->positionLocation = this->positionLocation;
        newMaterial->projectionMatrixLocation = this->projectionMatrixLocation;
        newMaterial->viewMatrixLocation = this->viewMatrixLocation;
        newMaterial->modelMatrixLocation = this->modelMatrixLocation;
        newMaterial->colorLocation = this->colorLocation;
        newMaterial->zOffsetLocation = this->zOffsetLocation;
        newMaterial->boneIndexLocation = this->boneIndexLocation;
        newMaterial->boneWeightLocation = this->boneWeightLocation;

        newMaterial->skinningEnabledLocation = this->skinningEnabledLocation;
        for (UInt32 i = 0; i < Constants::MaxBones; i++) {
          newMaterial->bonesLocation[i] = this->bonesLocation[i];
        }
        return newMaterial;
    }

    void BasicColoredMaterial::bindShaderVarLocations() {
        this->positionLocation = this->shader->getAttributeLocation(StandardAttribute::Position);
        this->projectionMatrixLocation = this->shader->getUniformLocation(StandardUniform::ProjectionMatrix);
        this->viewMatrixLocation = this->shader->getUniformLocation(StandardUniform::ViewMatrix);
        this->modelMatrixLocation = this->shader->getUniformLocation(StandardUniform::ModelMatrix);
        this->colorLocation = this->shader->getUniformLocation("color");
        this->zOffsetLocation = this->shader->getUniformLocation("zOffset");
        this->boneIndexLocation = this->shader->getAttributeLocation(StandardAttribute::BoneIndex);
        this->boneWeightLocation = this->shader->getAttributeLocation(StandardAttribute::BoneWeight);

        this->skinningEnabledLocation = this->shader->getUniformLocation(StandardUniform::SkinningEnabled);
        for (UInt32 i = 0; i < Constants::MaxBones; i++) {
          this->bonesLocation[i] = this->shader->getUniformLocation(StandardUniform::Bones, i);
        }
        
    }

    void BasicColoredMaterial::setColor(Color color) {
        this->color = color;
    }

    void BasicColoredMaterial::setZOffset(Real offset) {
        this->zOffset = offset;
    }
}