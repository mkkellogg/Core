#include "SpecularIBLBRDFRendererMaterial.h"
#include "../material/Shader.h"
#include "../util/WeakPointer.h"
#include "StandardAttributes.h"
#include "StandardUniforms.h"
#include "../Engine.h"
#include "../material/ShaderManager.h"

namespace Core {

    SpecularIBLBRDFRendererMaterial::SpecularIBLBRDFRendererMaterial(const std::string& vertShaderName, const std::string& fragShaderName, WeakPointer<Graphics> graphics): ShaderMaterial(vertShaderName, fragShaderName, graphics) {
    }
    
    SpecularIBLBRDFRendererMaterial::SpecularIBLBRDFRendererMaterial(WeakPointer<Graphics> graphics) : SpecularIBLBRDFRendererMaterial("SpecularIBLBRDFRenderer", "SpecularIBLBRDFRenderer", graphics) {
    }

    Bool SpecularIBLBRDFRendererMaterial::build() {
        if(!ShaderMaterial::build()) return false;
        this->bindShaderVarLocations();
        return true;
    }

    Int32 SpecularIBLBRDFRendererMaterial::getShaderLocation(StandardAttribute attribute, UInt32 offset) {
        switch (attribute) {
            case StandardAttribute::Position:
                return this->positionLocation;
            default:
                return -1;
        }
    }

    Int32 SpecularIBLBRDFRendererMaterial::getShaderLocation(StandardUniform uniform, UInt32 offset) {
        switch (uniform) {
            case StandardUniform::ProjectionMatrix:
                return this->projectionMatrixLocation;
            case StandardUniform::ViewMatrix:
                return this->viewMatrixLocation;
            case StandardUniform::ModelMatrix:
                return this->modelMatrixLocation;
            default:
                return -1;
        }
    }

    void SpecularIBLBRDFRendererMaterial::sendCustomUniformsToShader() {
    }

    void SpecularIBLBRDFRendererMaterial::copyTo(WeakPointer<Material> target) {
        ShaderMaterial::copyTo(target);
        WeakPointer<SpecularIBLBRDFRendererMaterial> _target = WeakPointer<Material>::dynamicPointerCast<SpecularIBLBRDFRendererMaterial>(target);
        _target->positionLocation = this->positionLocation;
        _target->projectionMatrixLocation = this->projectionMatrixLocation;
        _target->viewMatrixLocation = this->viewMatrixLocation;
        _target->modelMatrixLocation = this->modelMatrixLocation;
    }

    WeakPointer<Material> SpecularIBLBRDFRendererMaterial::clone() {
        WeakPointer<SpecularIBLBRDFRendererMaterial> newMaterial = Engine::instance()->createMaterial<SpecularIBLBRDFRendererMaterial>(false);
        this->copyTo(newMaterial);
        return newMaterial;
    }

    void SpecularIBLBRDFRendererMaterial::bindShaderVarLocations() {
        this->positionLocation = this->shader->getAttributeLocation(StandardAttribute::Position);
        this->projectionMatrixLocation = this->shader->getUniformLocation(StandardUniform::ProjectionMatrix);
        this->viewMatrixLocation = this->shader->getUniformLocation(StandardUniform::ViewMatrix);
        this->modelMatrixLocation = this->shader->getUniformLocation(StandardUniform::ModelMatrix);
    }
}
