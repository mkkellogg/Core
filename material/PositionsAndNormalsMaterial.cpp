#include "PositionsAndNormalsMaterial.h"
#include "../material/Shader.h"
#include "../Engine.h"
#include "../Graphics.h"
#include "../material/ShaderManager.h"

namespace Core {

    PositionsAndNormalsMaterial::PositionsAndNormalsMaterial() {
        this->convertToViewSpace = false;
    }

    PositionsAndNormalsMaterial::~PositionsAndNormalsMaterial() {
    }
    
    Bool PositionsAndNormalsMaterial::build() {
        WeakPointer<Graphics> graphics = Engine::instance()->getGraphicsSystem();
        ShaderManager& shaderManager = graphics->getShaderManager();
        this->shader = shaderManager.getShader("PositionsAndNormals");
        graphics->activateShader(this->shader);
        this->bindShaderVarLocations();
        return true;
    }

    void PositionsAndNormalsMaterial::sendCustomUniformsToShader() {
        this->shader->setUniform1i(this->viewSpaceLocation, this->convertToViewSpace ? 1 : 0);
    }

    void PositionsAndNormalsMaterial::copyTo(WeakPointer<Material> target) {
        WeakPointer<PositionsAndNormalsMaterial> positionsNormalsMaterial = WeakPointer<Material>::dynamicPointerCast<PositionsAndNormalsMaterial>(target);
        if (positionsNormalsMaterial.isValid()) {
            BaseMaterial::copyTo(target);
            positionsNormalsMaterial->viewSpaceLocation = this->viewSpaceLocation;
            positionsNormalsMaterial->convertToViewSpace = this->convertToViewSpace;
        } else {
            throw InvalidArgumentException("PositionsAndNormalsMaterial::copyTo() -> 'target must be same material.");
        }
    }

    WeakPointer<Material> PositionsAndNormalsMaterial::clone() {
        WeakPointer<PositionsAndNormalsMaterial> newMaterial = Engine::instance()->createMaterial<PositionsAndNormalsMaterial>(false);
        this->copyTo(newMaterial);
        return newMaterial;
    }

    void PositionsAndNormalsMaterial::bindShaderVarLocations() {
        BaseMaterial::bindShaderVarLocations();
        this->viewSpaceLocation = this->shader->getUniformLocation("viewSpace");
    }

    void PositionsAndNormalsMaterial::setConvertToViewSpace(Bool convertToViewSpace) {
        this->convertToViewSpace = convertToViewSpace;
    }
}
