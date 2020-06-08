#include "BaseMaterial.h"
#include "../material/Shader.h"
#include "StandardAttributes.h"
#include "StandardUniforms.h"
#include "../Engine.h"
#include "../Graphics.h"
#include "../material/ShaderManager.h"

namespace Core {

    BaseMaterial::BaseMaterial(WeakPointer<Graphics> graphics) : Material(graphics) {
        this->positionLocation = -1;
        this->normalLocation = -1;
        this->averagedNormalLocation = -1;
        this->faceNormalLocation = -1;
        this->colorLocation = -1;
        this->tangentLocation = -1;

        this->projectionMatrixLocation = -1;
        this->viewMatrixLocation = -1;
        this->modelMatrixLocation = -1;
        this->modelInverseTransposeMatrixLocation = -1;
        this->viewInverseTransposeMatrixLocation = -1;
        this->cameraPositionLocation = -1;

        this->skinningEnabledLocation = -1;
        for (UInt32 i = 0; i < Constants::MaxBones; i++) this->bonesLocation[i] = -1;
        this->boneIndexLocation = -1;
        this->boneWeightLocation = -1;
    }

    BaseMaterial::~BaseMaterial() {
    }

    Int32 BaseMaterial::getShaderLocation(StandardAttribute attribute, UInt32 offset) {
        switch (attribute) {
            case StandardAttribute::Position:
                return this->positionLocation;
            case StandardAttribute::Normal:
                return this->normalLocation;
            case StandardAttribute::AveragedNormal:
                return this->averagedNormalLocation;
            case StandardAttribute::FaceNormal:
                return this->faceNormalLocation;
            case StandardAttribute::Color:
                return this->colorLocation;
            case StandardAttribute::Tangent:
                return this->tangentLocation;
            case StandardAttribute::BoneIndex:
                return this->boneIndexLocation;
            case StandardAttribute::BoneWeight:
                return this->boneWeightLocation;
            default:
                return -1;
        }
    }

    Int32 BaseMaterial::getShaderLocation(StandardUniform uniform, UInt32 offset) {
        switch (uniform) {
            case StandardUniform::ProjectionMatrix:
                return this->projectionMatrixLocation;
            case StandardUniform::ViewMatrix:
                return this->viewMatrixLocation;
            case StandardUniform::ModelMatrix:
                return this->modelMatrixLocation;
            case StandardUniform::ModelInverseTransposeMatrix:
                return this->modelInverseTransposeMatrixLocation;
            case StandardUniform::ViewInverseTransposeMatrix:
                return this->viewInverseTransposeMatrixLocation;
            case StandardUniform::CameraPosition:
                return this->cameraPositionLocation;
            case StandardUniform::SkinningEnabled:
                return this->skinningEnabledLocation;
            case StandardUniform::Bones:
                return this->bonesLocation[offset];
            default:
                return -1;
        }
    }

    void BaseMaterial::copyTo(WeakPointer<Material> target) {
        WeakPointer<BaseMaterial> baseMaterial = WeakPointer<Material>::dynamicPointerCast<BaseMaterial>(target);
        if (baseMaterial.isValid()) {
            Material::copyTo(target);
            baseMaterial->positionLocation = this->positionLocation;
            baseMaterial->normalLocation = this->normalLocation;
            baseMaterial->averagedNormalLocation = this->averagedNormalLocation;
            baseMaterial->faceNormalLocation = this->faceNormalLocation;
            baseMaterial->colorLocation = this->colorLocation;
            baseMaterial->tangentLocation = this->tangentLocation;
            baseMaterial->projectionMatrixLocation = this->projectionMatrixLocation;
            baseMaterial->viewMatrixLocation = this->viewMatrixLocation;
            baseMaterial->modelMatrixLocation = this->modelMatrixLocation;
            baseMaterial->modelInverseTransposeMatrixLocation = this->modelInverseTransposeMatrixLocation;
            baseMaterial->viewInverseTransposeMatrixLocation = this->viewInverseTransposeMatrixLocation;
            baseMaterial->cameraPositionLocation = this->cameraPositionLocation;
            baseMaterial->boneIndexLocation = this->boneIndexLocation;
            baseMaterial->boneWeightLocation = this->boneWeightLocation;
            baseMaterial->skinningEnabledLocation = this->skinningEnabledLocation;
            for (UInt32 i = 0; i < Constants::MaxBones; i++) {
                baseMaterial->bonesLocation[i] = this->bonesLocation[i];
            }
        } else {
            throw InvalidArgumentException("BaseMaterial::copyTo() -> 'target must be same material.");
        }
    }

    void BaseMaterial::bindShaderVarLocations() {
        this->positionLocation = this->shader->getAttributeLocation(StandardAttribute::Position);
        this->normalLocation = this->shader->getAttributeLocation(StandardAttribute::Normal);
        this->averagedNormalLocation = this->shader->getAttributeLocation(StandardAttribute::AveragedNormal);
        this->faceNormalLocation = this->shader->getAttributeLocation(StandardAttribute::FaceNormal);
        this->colorLocation = this->shader->getAttributeLocation(StandardAttribute::Color);
        this->tangentLocation = this->shader->getAttributeLocation(StandardAttribute::Tangent);
        this->projectionMatrixLocation = this->shader->getUniformLocation(StandardUniform::ProjectionMatrix);
        this->viewMatrixLocation = this->shader->getUniformLocation(StandardUniform::ViewMatrix);
        this->modelMatrixLocation = this->shader->getUniformLocation(StandardUniform::ModelMatrix);
        this->modelInverseTransposeMatrixLocation = this->shader->getUniformLocation(StandardUniform::ModelInverseTransposeMatrix);
        this->viewInverseTransposeMatrixLocation = this->shader->getUniformLocation(StandardUniform::ViewInverseTransposeMatrix);
        this->cameraPositionLocation = this->shader->getUniformLocation(StandardUniform::CameraPosition);
        this->boneIndexLocation = this->shader->getAttributeLocation(StandardAttribute::BoneIndex);
        this->boneWeightLocation = this->shader->getAttributeLocation(StandardAttribute::BoneWeight);
        this->skinningEnabledLocation = this->shader->getUniformLocation(StandardUniform::SkinningEnabled);
        for (UInt32 i = 0; i < Constants::MaxBones; i++) {
          this->bonesLocation[i] = this->shader->getUniformLocation(StandardUniform::Bones, i);
        }
    }
}