#include "DepthOnlyMaterial.h"
#include "../material/Shader.h"
#include "../util/WeakPointer.h"
#include "StandardAttributes.h"
#include "StandardUniforms.h"
#include "../Engine.h"
#include "../material/ShaderManager.h"

namespace Core {

    DepthOnlyMaterial::DepthOnlyMaterial(WeakPointer<Graphics> graphics) : Material(graphics) {
    }

    Bool DepthOnlyMaterial::build() {
        WeakPointer<Graphics> graphics = Engine::instance()->getGraphicsSystem();
        ShaderManager& shaderDirectory = graphics->getShaderManager();
        const std::string& vertexSrc = shaderDirectory.getShader(ShaderType::Vertex, "Depth");
        const std::string& fragmentSrc = shaderDirectory.getShader(ShaderType::Fragment, "Depth");
        Bool ready = this->buildFromSource(vertexSrc, fragmentSrc);
        if (!ready) {
            return false;
        }

        this->bindShaderVarLocations();
        return true;
    }

    Int32 DepthOnlyMaterial::getShaderLocation(StandardAttribute attribute, UInt32 offset) {
        switch (attribute) {
            case StandardAttribute::Position:
                return this->positionLocation;
            default:
                return -1;
        }
    }

    Int32 DepthOnlyMaterial::getShaderLocation(StandardUniform uniform, UInt32 offset) {
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

    void DepthOnlyMaterial::sendCustomUniformsToShader() {
    }

    WeakPointer<Material> DepthOnlyMaterial::clone() {
        WeakPointer<DepthOnlyMaterial> newMaterial = Engine::instance()->createMaterial<DepthOnlyMaterial>(false);
        newMaterial->setShader(this->getShader());
        newMaterial->positionLocation = this->positionLocation;
        newMaterial->projectionMatrixLocation = this->projectionMatrixLocation;
        newMaterial->viewMatrixLocation = this->viewMatrixLocation;
        newMaterial->modelMatrixLocation = this->modelMatrixLocation;
        return newMaterial;
    }

    void DepthOnlyMaterial::bindShaderVarLocations() {
        this->positionLocation = this->shader->getAttributeLocation(StandardAttribute::Position);
        this->projectionMatrixLocation = this->shader->getUniformLocation("projection");
        this->viewMatrixLocation = this->shader->getUniformLocation("viewMatrix");
        this->modelMatrixLocation = this->shader->getUniformLocation(StandardUniform::ModelMatrix);
    }
}