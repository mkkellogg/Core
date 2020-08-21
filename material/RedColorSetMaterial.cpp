#include "RedColorSetMaterial.h"
#include "../material/Shader.h"
#include "../Engine.h"
#include "../Graphics.h"
#include "../material/ShaderManager.h"
#include "../image/Texture.h"

namespace Core {

    RedColorSetMaterial::RedColorSetMaterial(WeakPointer<Graphics> graphics) : BaseMaterial(graphics) {
        this->inputColor.set(1.0, 0.0, 1.0, 1.0);
        this->outputColor.set(1.0, 0.0, 1.0, 1.0);
    }

    RedColorSetMaterial::~RedColorSetMaterial() {
    }
    
    Bool RedColorSetMaterial::build() {
        WeakPointer<Graphics> graphics = Engine::instance()->getGraphicsSystem();
        ShaderManager& shaderManager = graphics->getShaderManager();
        this->shader = shaderManager.getShader("RedColorSet");
        graphics->activateShader(this->shader);
        this->bindShaderVarLocations();
        return true;
    }

    Int32 RedColorSetMaterial::getShaderLocation(StandardUniform uniform, UInt32 offset) {
        Int32 uniformLocation = BaseMaterial::getShaderLocation(uniform, offset);
        if (uniformLocation >= 0) return uniformLocation;
        switch (uniform) {
            case StandardUniform::Texture0:
                return this->inputLocation;
            default:
                return -1;
        }
    }

    void RedColorSetMaterial::sendCustomUniformsToShader() {
        this->shader->setUniform4f(this->inputColorLocation, this->inputColor.r, this->inputColor.g, this->inputColor.b, this->inputColor.a);
        this->shader->setUniform4f(this->outputColorLocation, this->outputColor.r, this->outputColor.g, this->outputColor.b, this->outputColor.a);
    }

    WeakPointer<Material> RedColorSetMaterial::clone() {
        WeakPointer<RedColorSetMaterial> newMaterial = Engine::instance()->createMaterial<RedColorSetMaterial>(false);
        this->copyTo(newMaterial);
        return newMaterial;
    }

     void RedColorSetMaterial::copyTo(WeakPointer<Material> target) {
        WeakPointer<RedColorSetMaterial> colorSetMaterial = WeakPointer<Material>::dynamicPointerCast<RedColorSetMaterial>(target);
        if (colorSetMaterial.isValid()) {
            BaseMaterial::copyTo(target);
            colorSetMaterial->inputLocation = this->inputLocation;
            colorSetMaterial->inputColorLocation = this->inputColorLocation;
            colorSetMaterial->inputColor = this->inputColor;
            colorSetMaterial->outputColorLocation = this->outputColorLocation;
            colorSetMaterial->outputColor = this->outputColor;
        } else {
            throw InvalidArgumentException("RedColorSetMaterial::copyTo() -> 'target must be same material.");
        }
    }

    void RedColorSetMaterial::bindShaderVarLocations() {
        BaseMaterial::bindShaderVarLocations();
        this->inputLocation = this->shader->getUniformLocation(StandardUniform::Texture0);
        this->inputColorLocation = this->shader->getUniformLocation("inputColor");
        this->outputColorLocation = this->shader->getUniformLocation("outputColor");
    }

    void RedColorSetMaterial::setInputColor(Color inputColor) {  
        this->inputColor = inputColor;
    } 

    void RedColorSetMaterial::setOutputColor(Color outputColor) {
        this->outputColor = outputColor;
    } 

}
