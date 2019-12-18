#include "BasicExtrusionMaterial.h"
#include "../material/Shader.h"
#include "../Engine.h"
#include "../Graphics.h"
#include "../material/ShaderManager.h"

namespace Core {

    BasicExtrusionMaterial::BasicExtrusionMaterial(WeakPointer<Graphics> graphics) : BaseMaterial(graphics) {
        this->objectColorLocation = -1;
        this->averagedNormalLocation = -1;
        this->zOffsetLocation = -1;
        this->extrusionFactorLocation = -1;

         this->zOffset = 0.0f;
         this->extrusionFactor = 0.5;
         this->objectColor.set(1.0, 1.0, 1.0, 1.0);
    }

    void BasicExtrusionMaterial::setObjectColor(Color color) {
        this->objectColor = color;
    }

    void BasicExtrusionMaterial::setZOffset(Real offset) {
        this->zOffset = offset;
    }

    void BasicExtrusionMaterial::setExtrusionFactor(Real extrusionFactor) {
        this->extrusionFactor = extrusionFactor;
    }

    Bool BasicExtrusionMaterial::build() {
        WeakPointer<Graphics> graphics = Engine::instance()->getGraphicsSystem();
        ShaderManager& shaderManager = graphics->getShaderManager();
        this->shader = shaderManager.getShader("BasicExtrusion");
        graphics->activateShader(this->shader);
        this->bindShaderVarLocations();
        return true;
    }

    void BasicExtrusionMaterial::sendCustomUniformsToShader() {
        this->shader->setUniform4f(this->objectColorLocation, this->objectColor.r, this->objectColor.g, this->objectColor.b, this->objectColor.a);
        this->shader->setUniform1f(this->zOffsetLocation, this->zOffset);
        this->shader->setUniform1f(this->extrusionFactorLocation, this->extrusionFactor);
    }

     void BasicExtrusionMaterial::copyTo(WeakPointer<Material> target) {
        WeakPointer<BasicExtrusionMaterial> basicExtrusionMaterial = WeakPointer<Material>::dynamicPointerCast<BasicExtrusionMaterial>(target);
        if (basicExtrusionMaterial.isValid()) {
            BaseMaterial::copyTo(target);
            basicExtrusionMaterial->objectColorLocation = this->objectColorLocation;
            basicExtrusionMaterial->zOffsetLocation = this->zOffsetLocation;
            basicExtrusionMaterial->extrusionFactorLocation = this->extrusionFactorLocation;
        } else {
            throw InvalidArgumentException("BasicExtrusionMaterial::copyTo() -> 'target must be same material.");
        }
     }

    WeakPointer<Material> BasicExtrusionMaterial::clone() {
        WeakPointer<BasicExtrusionMaterial> newMaterial = Engine::instance()->createMaterial<BasicExtrusionMaterial>(false);
        this->copyTo(newMaterial);
        return newMaterial;
    }

    void BasicExtrusionMaterial::bindShaderVarLocations() {
        BaseMaterial::bindShaderVarLocations();
        this->objectColorLocation = this->shader->getUniformLocation("objectColor");
        this->extrusionFactorLocation = this->shader->getUniformLocation("extrusionFactor");
        this->zOffsetLocation = this->shader->getUniformLocation("zOffset");
    }
}