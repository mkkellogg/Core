#include "DistanceOnlyMaterial.h"
#include "../material/Shader.h"
#include "../util/WeakPointer.h"
#include "StandardAttributes.h"
#include "StandardUniforms.h"
#include "../Engine.h"
#include "../material/ShaderManager.h"

namespace Core {

    DistanceOnlyMaterial::DistanceOnlyMaterial(WeakPointer<Graphics> graphics) : Material(graphics) {
    }

    Bool DistanceOnlyMaterial::build() {
        WeakPointer<Graphics> graphics = Engine::instance()->getGraphicsSystem();
        ShaderManager& shaderDirectory = graphics->getShaderManager();
        const std::string& vertexSrc = shaderDirectory.getShader(ShaderType::Vertex, "Distance");
        const std::string& fragmentSrc = shaderDirectory.getShader(ShaderType::Fragment, "Distance");
        Bool ready = this->buildFromSource(vertexSrc, fragmentSrc);
        if (!ready) {
            return false;
        }

        this->bindShaderVarLocations();
        return true;
    }

    Int32 DistanceOnlyMaterial::getShaderLocation(StandardAttribute attribute, UInt32 offset) {
        switch (attribute) {
            case StandardAttribute::Position:
                return this->positionLocation;
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
            default:
                return -1;
        }
    }

    void DistanceOnlyMaterial::sendCustomUniformsToShader() {
    }

    WeakPointer<Material> DistanceOnlyMaterial::clone() {
        WeakPointer<DistanceOnlyMaterial> newMaterial = Engine::instance()->createMaterial<DistanceOnlyMaterial>(false);
        newMaterial->setShader(this->getShader());
        newMaterial->positionLocation = this->positionLocation;
        newMaterial->projectionMatrixLocation = this->projectionMatrixLocation;
        newMaterial->viewMatrixLocation = this->viewMatrixLocation;
        newMaterial->modelMatrixLocation = this->modelMatrixLocation;
        return newMaterial;
    }

    void DistanceOnlyMaterial::bindShaderVarLocations() {
        this->positionLocation = this->shader->getAttributeLocation("pos");
        this->projectionMatrixLocation = this->shader->getUniformLocation("projection");
        this->viewMatrixLocation = this->shader->getUniformLocation("viewMatrix");
        this->modelMatrixLocation = this->shader->getUniformLocation(StandardUniform::ModelMatrix);
    }
}