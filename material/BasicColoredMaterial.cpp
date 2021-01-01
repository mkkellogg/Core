#include "BasicColoredMaterial.h"
#include "../material/Shader.h"
#include "../Engine.h"
#include "../Graphics.h"
#include "../material/ShaderManager.h"

namespace Core {

    BasicColoredMaterial::BasicColoredMaterial() {
        this->objectColorLocation = -1;
        this->zOffsetLocation = -1;
        this->zOffset = 0.0f;
    }

    BasicColoredMaterial::~BasicColoredMaterial() {
    }

    void BasicColoredMaterial::setObjectColor(Color color) {
        this->objectColor = color;
    }

    void BasicColoredMaterial::setZOffset(Real offset) {
        this->zOffset = offset;
    }

    Bool BasicColoredMaterial::build() {
        WeakPointer<Graphics> graphics = Engine::instance()->getGraphicsSystem();
        ShaderManager& shaderManager = graphics->getShaderManager();
        this->shader = shaderManager.getShader("BasicColored");
        graphics->activateShader(this->shader);
        this->bindShaderVarLocations();
        return true;
    }

    void BasicColoredMaterial::sendCustomUniformsToShader() {
        this->shader->setUniform4f(this->objectColorLocation, this->objectColor.r, this->objectColor.g, this->objectColor.b, this->objectColor.a);
        this->shader->setUniform1f(this->zOffsetLocation, this->zOffset);
    }

     void BasicColoredMaterial::copyTo(WeakPointer<Material> target) {
        WeakPointer<BasicColoredMaterial> basicColoredMaterial = WeakPointer<Material>::dynamicPointerCast<BasicColoredMaterial>(target);
        if (basicColoredMaterial.isValid()) {
            BaseMaterial::copyTo(target);
            basicColoredMaterial->objectColorLocation = this->objectColorLocation;
            basicColoredMaterial->zOffsetLocation = this->zOffsetLocation;
        } else {
            throw InvalidArgumentException("BasicColoredMaterial::copyTo() -> 'target must be same material.");
        }
    }

    WeakPointer<Material> BasicColoredMaterial::clone() {
        WeakPointer<BasicColoredMaterial> newMaterial = Engine::instance()->createMaterial<BasicColoredMaterial>(false);
        this->copyTo(newMaterial);
        return newMaterial;
    }

    void BasicColoredMaterial::bindShaderVarLocations() {
        BaseMaterial::bindShaderVarLocations();
        this->objectColorLocation = this->shader->getUniformLocation("objectColor");
        this->zOffsetLocation = this->shader->getUniformLocation("zOffset");
    }
}