#include "OutlineMaterial.h"
#include "../material/Shader.h"
#include "../util/WeakPointer.h"
#include "StandardAttributes.h"
#include "StandardUniforms.h"
#include "../Engine.h"
#include "../material/ShaderManager.h"

namespace Core {

    OutlineMaterial::OutlineMaterial(WeakPointer<Graphics> graphics) : Material(graphics) {
        this->zOffset = 0.0f;
    }

    Bool OutlineMaterial::build() {
        WeakPointer<Graphics> graphics = Engine::instance()->getGraphicsSystem();
        ShaderManager& shaderDirectory = graphics->getShaderManager();
        const std::string& vertexSrc = shaderDirectory.getShader(ShaderType::Vertex, "Outline");
        const std::string& geometrySrc = shaderDirectory.getShader(ShaderType::Geometry, "Outline");
        const std::string& fragmentSrc = shaderDirectory.getShader(ShaderType::Fragment, "Outline");
        Bool ready = this->buildFromSource(vertexSrc, geometrySrc, fragmentSrc);
        if (!ready) {
            return false;
        }

        this->bindShaderVarLocations();
        return true;
    }

    Int32 OutlineMaterial::getShaderLocation(StandardAttribute attribute, UInt32 offset) {
        switch (attribute) {
            case StandardAttribute::Position:
                return this->positionLocation;
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
            default:
                return -1;
        }
    }

    void OutlineMaterial::sendCustomUniformsToShader() {
        this->shader->setUniform4f(this->colorLocation, this->color.r, this->color.g, this->color.b, this->color.a);
        this->shader->setUniform1f(this->zOffsetLocation, this->zOffset);
    }

    WeakPointer<Material> OutlineMaterial::clone() {
        WeakPointer<OutlineMaterial> newMaterial = Engine::instance()->createMaterial<OutlineMaterial>(false);
        newMaterial->setShader(this->getShader());
        newMaterial->positionLocation = this->positionLocation;
        newMaterial->projectionMatrixLocation = this->projectionMatrixLocation;
        newMaterial->viewMatrixLocation = this->viewMatrixLocation;
        newMaterial->modelMatrixLocation = this->modelMatrixLocation;
        newMaterial->colorLocation = this->colorLocation;
        newMaterial->zOffsetLocation = this->zOffsetLocation;
        return newMaterial;
    }

    void OutlineMaterial::bindShaderVarLocations() {
        this->positionLocation = this->shader->getAttributeLocation(StandardAttribute::Position);
        this->projectionMatrixLocation = this->shader->getUniformLocation(StandardUniform::ProjectionMatrix);
        this->viewMatrixLocation = this->shader->getUniformLocation(StandardUniform::ViewMatrix);
        this->modelMatrixLocation = this->shader->getUniformLocation(StandardUniform::ModelMatrix);
        this->colorLocation = this->shader->getUniformLocation("color");
        this->zOffsetLocation = this->shader->getUniformLocation("zOffset");
        
    }

    void OutlineMaterial::setColor(Color color) {
        this->color = color;
    }

    void OutlineMaterial::setZOffset(Real offset) {
        this->zOffset = offset;
    }
}