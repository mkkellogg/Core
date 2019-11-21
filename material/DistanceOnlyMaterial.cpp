#include "DistanceOnlyMaterial.h"
#include "../material/Shader.h"
#include "../util/WeakPointer.h"
#include "StandardAttributes.h"
#include "StandardUniforms.h"
#include "../Engine.h"
#include "../Graphics.h"
#include "../material/ShaderManager.h"

namespace Core {

    DistanceOnlyMaterial::DistanceOnlyMaterial(WeakPointer<Graphics> graphics) : Material(graphics) {
    }

    Bool DistanceOnlyMaterial::build() {
        WeakPointer<Graphics> graphics = Engine::instance()->getGraphicsSystem();
        ShaderManager& shaderManager = graphics->getShaderManager();
        this->shader = shaderManager.getShader("Distance");
        graphics->activateShader(this->shader);
        this->bindShaderVarLocations();
        this->setSkinningEnabled(true);
        return true;
    }

    Int32 DistanceOnlyMaterial::getShaderLocation(StandardAttribute attribute, UInt32 offset) {
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

    Int32 DistanceOnlyMaterial::getShaderLocation(StandardUniform uniform, UInt32 offset) {
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

    void DistanceOnlyMaterial::sendCustomUniformsToShader() {
    }

    WeakPointer<Material> DistanceOnlyMaterial::clone() {
        WeakPointer<DistanceOnlyMaterial> newMaterial = Engine::instance()->createMaterial<DistanceOnlyMaterial>(false);
        this->copyTo(newMaterial);
        newMaterial->positionLocation = this->positionLocation;
        newMaterial->projectionMatrixLocation = this->projectionMatrixLocation;
        newMaterial->viewMatrixLocation = this->viewMatrixLocation;
        newMaterial->modelMatrixLocation = this->modelMatrixLocation;
        newMaterial->boneIndexLocation = this->boneIndexLocation;
        newMaterial->boneWeightLocation = this->boneWeightLocation;

        newMaterial->skinningEnabledLocation = this->skinningEnabledLocation;
        for (UInt32 i = 0; i < Constants::MaxBones; i++) {
          newMaterial->bonesLocation[i] = this->bonesLocation[i];
        }
        return newMaterial;
    }

    void DistanceOnlyMaterial::bindShaderVarLocations() {
        this->positionLocation = this->shader->getAttributeLocation(StandardAttribute::Position);
        this->projectionMatrixLocation = this->shader->getUniformLocation(StandardUniform::ProjectionMatrix);
        this->viewMatrixLocation = this->shader->getUniformLocation(StandardUniform::ViewMatrix);
        this->modelMatrixLocation = this->shader->getUniformLocation(StandardUniform::ModelMatrix);
        this->boneIndexLocation = this->shader->getAttributeLocation(StandardAttribute::BoneIndex);
        this->boneWeightLocation = this->shader->getAttributeLocation(StandardAttribute::BoneWeight);

        this->skinningEnabledLocation = this->shader->getUniformLocation(StandardUniform::SkinningEnabled);
        for (UInt32 i = 0; i < Constants::MaxBones; i++) {
          this->bonesLocation[i] = this->shader->getUniformLocation(StandardUniform::Bones, i);
        }
    }
}