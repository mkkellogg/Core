#include "CopyMaterial.h"
#include "../material/Shader.h"
#include "../Engine.h"
#include "../Graphics.h"
#include "../material/ShaderManager.h"
#include "../image/Texture.h"

namespace Core {

    CopyMaterial::CopyMaterial(WeakPointer<Graphics> graphics) : BaseMaterial(graphics) {
    }

    CopyMaterial::~CopyMaterial() {
    }
    
    Bool CopyMaterial::build() {
        WeakPointer<Graphics> graphics = Engine::instance()->getGraphicsSystem();
        ShaderManager& shaderManager = graphics->getShaderManager();
        this->shader = shaderManager.getShader("Copy");
        graphics->activateShader(this->shader);
        this->bindShaderVarLocations();
        return true;
    }
    
    Int32 CopyMaterial::getShaderLocation(StandardUniform uniform, UInt32 offset) {
        Int32 uniformLocation = BaseMaterial::getShaderLocation(uniform, offset);
        if (uniformLocation >= 0) return uniformLocation;
        switch (uniform) {
            case StandardUniform::Texture0:
                return this->textureLocation;
            default:
                return -1;
        }
    }

    WeakPointer<Material> CopyMaterial::clone() {
        WeakPointer<CopyMaterial> newMaterial = Engine::instance()->createMaterial<CopyMaterial>(false);
        this->copyTo(newMaterial);
        return newMaterial;
    }

     void CopyMaterial::copyTo(WeakPointer<Material> target) {
        WeakPointer<CopyMaterial> copyMaterial = WeakPointer<Material>::dynamicPointerCast<CopyMaterial>(target);
        if (copyMaterial.isValid()) {
            BaseMaterial::copyTo(target);
            copyMaterial->textureLocation = this->textureLocation;
        } else {
            throw InvalidArgumentException("CopyMaterial::copyTo() -> 'target must be same material.");
        }
    }

    void CopyMaterial::bindShaderVarLocations() {
        BaseMaterial::bindShaderVarLocations();
        this->textureLocation = this->shader->getUniformLocation(StandardUniform::Texture0);
    }
}
