#include "OutlineMaterial.h"
#include "../material/Shader.h"
#include "../util/WeakPointer.h"
#include "StandardAttributes.h"
#include "StandardUniforms.h"
#include "../Engine.h"
#include "../Graphics.h"
#include "../material/ShaderManager.h"

namespace Core {

    OutlineMaterial::OutlineMaterial(WeakPointer<Graphics> graphics) : Material(graphics) {
        this->color.set(1.0, 0.0, 1.0, 1.0);
        this->edgeWidth = 0.005;
        this->pctExtend = 0.0;
        this->absExtend = 0.0025;
    }

    Bool OutlineMaterial::build() {
        WeakPointer<Graphics> graphics = Engine::instance()->getGraphicsSystem();
        ShaderManager& shaderManager = graphics->getShaderManager();
        this->shader = shaderManager.getShader("Outline");
        graphics->activateShader(this->shader);
        this->bindShaderVarLocations();
        this->setSkinningEnabled(true);
        return true;
    }

    Int32 OutlineMaterial::getShaderLocation(StandardAttribute attribute, UInt32 offset) {
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

    Int32 OutlineMaterial::getShaderLocation(StandardUniform uniform, UInt32 offset) {
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

    void OutlineMaterial::sendCustomUniformsToShader() {
        this->shader->setUniform4f(this->colorLocation, this->color.r, this->color.g, this->color.b, this->color.a);
        this->shader->setUniform1f(this->edgeWidthLocation, this->edgeWidth);
        this->shader->setUniform1f(this->pctExtendLocation, this->pctExtend);
        this->shader->setUniform1f(this->absExtendLocation, this->absExtend);
        
    }

    WeakPointer<Material> OutlineMaterial::clone() {
        WeakPointer<OutlineMaterial> newMaterial = Engine::instance()->createMaterial<OutlineMaterial>(false);
        this->copyTo(newMaterial);
        newMaterial->positionLocation = this->positionLocation;
        newMaterial->projectionMatrixLocation = this->projectionMatrixLocation;
        newMaterial->viewMatrixLocation = this->viewMatrixLocation;
        newMaterial->modelMatrixLocation = this->modelMatrixLocation;
        newMaterial->colorLocation = this->colorLocation;
        newMaterial->edgeWidthLocation = this->edgeWidthLocation;
        newMaterial->pctExtendLocation = this->pctExtendLocation;
        newMaterial->absExtendLocation = this->absExtendLocation;
        newMaterial->boneIndexLocation = this->boneIndexLocation;
        newMaterial->boneWeightLocation = this->boneWeightLocation;

        newMaterial->skinningEnabledLocation = this->skinningEnabledLocation;
        for (UInt32 i = 0; i < Constants::MaxBones; i++) {
          newMaterial->bonesLocation[i] = this->bonesLocation[i];
        }
        return newMaterial;
    }

    void OutlineMaterial::bindShaderVarLocations() {
        this->positionLocation = this->shader->getAttributeLocation(StandardAttribute::Position);
        this->projectionMatrixLocation = this->shader->getUniformLocation(StandardUniform::ProjectionMatrix);
        this->viewMatrixLocation = this->shader->getUniformLocation(StandardUniform::ViewMatrix);
        this->modelMatrixLocation = this->shader->getUniformLocation(StandardUniform::ModelMatrix);
        this->colorLocation = this->shader->getUniformLocation("color");
        this->edgeWidthLocation = this->shader->getUniformLocation("edgeWidth");
        this->pctExtendLocation = this->shader->getUniformLocation("pctExtend");
        this->absExtendLocation = this->shader->getUniformLocation("absExtend");
        this->boneIndexLocation = this->shader->getAttributeLocation(StandardAttribute::BoneIndex);
        this->boneWeightLocation = this->shader->getAttributeLocation(StandardAttribute::BoneWeight);

        this->skinningEnabledLocation = this->shader->getUniformLocation(StandardUniform::SkinningEnabled);
        for (UInt32 i = 0; i < Constants::MaxBones; i++) {
          this->bonesLocation[i] = this->shader->getUniformLocation(StandardUniform::Bones, i);
        }
    }

    void OutlineMaterial::setColor(Color color) {
        this->color = color;
    }

    void OutlineMaterial::setEdgeWidth(Real width) {
        this->edgeWidth = width;
    }

    void OutlineMaterial::setPctExtend(Real extend) {
        this->pctExtend = extend;
    }

    void OutlineMaterial::setAbsExtend(Real extend) {
        this->absExtend = extend;
    }
}