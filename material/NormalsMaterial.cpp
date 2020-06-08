#include "NormalsMaterial.h"
#include "../material/Shader.h"
#include "../Engine.h"
#include "../Graphics.h"
#include "../material/ShaderManager.h"

namespace Core {

    NormalsMaterial::NormalsMaterial(WeakPointer<Graphics> graphics) : BaseMaterial(graphics) {
        this->convertToViewSpace = false;
    }

    NormalsMaterial::~NormalsMaterial() {
    }
    
    Bool NormalsMaterial::build() {
        WeakPointer<Graphics> graphics = Engine::instance()->getGraphicsSystem();
        ShaderManager& shaderManager = graphics->getShaderManager();
        this->shader = shaderManager.getShader("Normals");
        graphics->activateShader(this->shader);
        this->bindShaderVarLocations();
        this->setSkinningEnabled(true);
        return true;
    }

    void NormalsMaterial::sendCustomUniformsToShader() {
        this->shader->setUniform1i(this->viewSpaceLocation, this->convertToViewSpace ? 1 : 0);
    }

    void NormalsMaterial::copyTo(WeakPointer<Material> target) {
        WeakPointer<NormalsMaterial> normalsMaterial = WeakPointer<Material>::dynamicPointerCast<NormalsMaterial>(target);
        if (normalsMaterial.isValid()) {
            BaseMaterial::copyTo(target);
            normalsMaterial->viewSpaceLocation = this->viewSpaceLocation;
            normalsMaterial->convertToViewSpace = this->convertToViewSpace;
        } else {
            throw InvalidArgumentException("NormalsMaterial::copyTo() -> 'target must be same material.");
        }
    }

    WeakPointer<Material> NormalsMaterial::clone() {
        WeakPointer<NormalsMaterial> newMaterial = Engine::instance()->createMaterial<NormalsMaterial>(false);
        this->copyTo(newMaterial);
        return newMaterial;
    }

    void NormalsMaterial::bindShaderVarLocations() {
        BaseMaterial::bindShaderVarLocations();
        this->viewSpaceLocation = this->shader->getUniformLocation("viewSpace");
    }

    void NormalsMaterial::setConvertToViewSpace(Bool convertToViewSpace) {
        this->convertToViewSpace = convertToViewSpace;
    }
}
