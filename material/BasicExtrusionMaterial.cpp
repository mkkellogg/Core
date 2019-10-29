#include "BasicExtrusionMaterial.h"
#include "../material/Shader.h"
#include "../util/WeakPointer.h"
#include "StandardAttributes.h"
#include "StandardUniforms.h"
#include "../Engine.h"
#include "../Graphics.h"
#include "../material/ShaderManager.h"

namespace Core {

    BasicExtrusionMaterial::BasicExtrusionMaterial(WeakPointer<Graphics> graphics) : Material(graphics) {
    }

    Bool BasicExtrusionMaterial::build() {
        WeakPointer<Graphics> graphics = Engine::instance()->getGraphicsSystem();
        ShaderManager& shaderDirectory = graphics->getShaderManager();
        const std::string& vertexSrc = shaderDirectory.getShader(ShaderType::Vertex, "BasicExtrusion");
        const std::string& fragmentSrc = shaderDirectory.getShader(ShaderType::Fragment, "BasicExtrusion");
        Bool ready = this->buildFromSource(vertexSrc, fragmentSrc);
        if (!ready) {
            return false;
        }

        this->bindShaderVarLocations();
        return true;
    }

    Int32 BasicExtrusionMaterial::getShaderLocation(StandardAttribute attribute, UInt32 offset) {
        switch (attribute) {
            case StandardAttribute::Position:
                return this->positionLocation;
             case StandardAttribute::Normal:
                return this->normalLocation;
            case StandardAttribute::AveragedNormal:
                return this->averagedNormalLocation;
            default:
                return -1;
        }
    }

    Int32 BasicExtrusionMaterial::getShaderLocation(StandardUniform uniform, UInt32 offset) {
        switch (uniform) {
            case StandardUniform::ProjectionMatrix:
                return this->projectionMatrixLocation;
            case StandardUniform::ViewMatrix:
                return this->viewMatrixLocation;
            case StandardUniform::ModelMatrix:
                return this->modelMatrixLocation;
            case StandardUniform::ModelInverseTransposeMatrix:
                return this->modelInverseTransposeMatrixLocation;
            default:
                return -1;
        }
    }

    void BasicExtrusionMaterial::sendCustomUniformsToShader() {
        this->shader->setUniform4f(this->colorLocation, this->color.r, this->color.g, this->color.b, this->color.a);
        this->shader->setUniform1f(this->zOffsetLocation, this->zOffset);
        this->shader->setUniform1f(this->extrusionFactorLocation, this->extrusionFactor);
    }

    WeakPointer<Material> BasicExtrusionMaterial::clone() {
        WeakPointer<BasicExtrusionMaterial> newMaterial = Engine::instance()->createMaterial<BasicExtrusionMaterial>(false);
        this->copyTo(newMaterial);
        newMaterial->positionLocation = this->positionLocation;
        newMaterial->normalLocation = this->normalLocation;
        newMaterial->averagedNormalLocation = this->averagedNormalLocation;
        newMaterial->colorLocation = this->colorLocation;
        newMaterial->projectionMatrixLocation = this->projectionMatrixLocation;
        newMaterial->viewMatrixLocation = this->viewMatrixLocation;
        newMaterial->modelMatrixLocation = this->modelMatrixLocation;
        newMaterial->modelInverseTransposeMatrixLocation = this->modelInverseTransposeMatrixLocation;
        newMaterial->colorLocation = this->colorLocation;
        newMaterial->zOffsetLocation = this->zOffsetLocation;
        newMaterial->extrusionFactorLocation = this->extrusionFactorLocation;
        return newMaterial;
    }

    void BasicExtrusionMaterial::bindShaderVarLocations() {
        this->positionLocation = this->shader->getAttributeLocation(StandardAttribute::Position);
        this->normalLocation = this->shader->getAttributeLocation(StandardAttribute::Normal);
        this->averagedNormalLocation = this->shader->getAttributeLocation(StandardAttribute::AveragedNormal);
        this->projectionMatrixLocation = this->shader->getUniformLocation(StandardUniform::ProjectionMatrix);
        this->viewMatrixLocation = this->shader->getUniformLocation(StandardUniform::ViewMatrix);
        this->modelMatrixLocation = this->shader->getUniformLocation(StandardUniform::ModelMatrix);
        this->modelInverseTransposeMatrixLocation = this->shader->getUniformLocation(StandardUniform::ModelInverseTransposeMatrix);
        this->colorLocation = this->shader->getUniformLocation("color");
        this->extrusionFactorLocation = this->shader->getUniformLocation("extrusionFactor");
        this->zOffsetLocation = this->shader->getUniformLocation("zOffset");
    }

    void BasicExtrusionMaterial::setColor(Color color) {
        this->color = color;
    }

    void BasicExtrusionMaterial::setZOffset(Real offset) {
        this->zOffset = offset;
    }

    void BasicExtrusionMaterial::setExtrusionFactor(Real extrusionFactor) {
        this->extrusionFactor = extrusionFactor;
    }
}