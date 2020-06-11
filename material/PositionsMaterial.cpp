#include "PositionsMaterial.h"
#include "../material/Shader.h"
#include "../Engine.h"
#include "../Graphics.h"
#include "../material/ShaderManager.h"

namespace Core {

    PositionsMaterial::PositionsMaterial(WeakPointer<Graphics> graphics) : BaseMaterial(graphics) {
        this->convertToViewSpace = false;
    }

    PositionsMaterial::~PositionsMaterial() {
    }
    
    Bool PositionsMaterial::build() {
        WeakPointer<Graphics> graphics = Engine::instance()->getGraphicsSystem();
        ShaderManager& shaderManager = graphics->getShaderManager();
        this->shader = shaderManager.getShader("Positions");
        graphics->activateShader(this->shader);
        this->bindShaderVarLocations();
        this->setSkinningEnabled(true);
        return true;
    }

    void PositionsMaterial::sendCustomUniformsToShader() {
        this->shader->setUniform1i(this->viewSpaceLocation, this->convertToViewSpace ? 1 : 0);
    }

    void PositionsMaterial::copyTo(WeakPointer<Material> target) {
        WeakPointer<PositionsMaterial> positionsMaterial = WeakPointer<Material>::dynamicPointerCast<PositionsMaterial>(target);
        if (positionsMaterial.isValid()) {
            BaseMaterial::copyTo(target);
            positionsMaterial->viewSpaceLocation = this->viewSpaceLocation;
            positionsMaterial->convertToViewSpace = this->convertToViewSpace;
        } else {
            throw InvalidArgumentException("PositionsMaterial::copyTo() -> 'target must be same material.");
        }
    }

    WeakPointer<Material> PositionsMaterial::clone() {
        WeakPointer<PositionsMaterial> newMaterial = Engine::instance()->createMaterial<PositionsMaterial>(false);
        this->copyTo(newMaterial);
        return newMaterial;
    }

    void PositionsMaterial::bindShaderVarLocations() {
        BaseMaterial::bindShaderVarLocations();
        this->viewSpaceLocation = this->shader->getUniformLocation("viewSpace");
    }

    void PositionsMaterial::setConvertToViewSpace(Bool convertToViewSpace) {
        this->convertToViewSpace = convertToViewSpace;
    }
}
