#include "BufferOutlineMaterial.h"
#include "../material/Shader.h"
#include "../Engine.h"
#include "../Graphics.h"
#include "../material/ShaderManager.h"
#include "../image/Texture.h"

namespace Core {

    BufferOutlineMaterial::BufferOutlineMaterial() {
        this->outlineColorLocation = -1;
        this->outlineSizeLocation = -1;
        this->silhouetteLocation = -1;

        this->outlineColor.set(1.0, 0.0, 1.0, 1.0);
        this->outlineSize = 2;
    }

    BufferOutlineMaterial::~BufferOutlineMaterial() {
    }
    
    Bool BufferOutlineMaterial::build() {
        WeakPointer<Graphics> graphics = Engine::instance()->getGraphicsSystem();
        ShaderManager& shaderManager = graphics->getShaderManager();
        this->shader = shaderManager.getShader("BufferOutline");
        graphics->activateShader(this->shader);
        this->bindShaderVarLocations();
        return true;
    }
    
    Int32 BufferOutlineMaterial::getShaderLocation(StandardUniform uniform, UInt32 offset) {
        Int32 uniformLocation = BaseMaterial::getShaderLocation(uniform, offset);
        if (uniformLocation >= 0) return uniformLocation;
        switch (uniform) {
            case StandardUniform::Texture0:
                return this->silhouetteLocation;
            default:
                return -1;
        }
    }

    void BufferOutlineMaterial::sendCustomUniformsToShader() {
        this->shader->setUniform4f(this->outlineColorLocation, this->outlineColor.r, this->outlineColor.g, this->outlineColor.b, this->outlineColor.a);
        this->shader->setUniform1i(this->outlineSizeLocation, this->outlineSize);
    }

    WeakPointer<Material> BufferOutlineMaterial::clone() {
        WeakPointer<BufferOutlineMaterial> newMaterial = Engine::instance()->createMaterial<BufferOutlineMaterial>(false);
        this->copyTo(newMaterial);
        return newMaterial;
    }

     void BufferOutlineMaterial::copyTo(WeakPointer<Material> target) {
        WeakPointer<BufferOutlineMaterial> bufferOutlineMaterial = WeakPointer<Material>::dynamicPointerCast<BufferOutlineMaterial>(target);
        if (bufferOutlineMaterial.isValid()) {
            BaseMaterial::copyTo(target);
            bufferOutlineMaterial->silhouetteLocation = this->silhouetteLocation;
            bufferOutlineMaterial->outlineColorLocation = this->outlineColorLocation;
            bufferOutlineMaterial->outlineSizeLocation = this->outlineSizeLocation;
            bufferOutlineMaterial->outlineColor = this->outlineColor;
            bufferOutlineMaterial->outlineSize = this->outlineSize;
        } else {
            throw InvalidArgumentException("BufferOutlineMaterial::copyTo() -> 'target must be same material.");
        }
    }

    void BufferOutlineMaterial::bindShaderVarLocations() {
        BaseMaterial::bindShaderVarLocations();
        this->outlineColorLocation = this->shader->getUniformLocation("outlineColor");
        this->outlineSizeLocation = this->shader->getUniformLocation("outlineSize");
        this->silhouetteLocation = this->shader->getUniformLocation(StandardUniform::Texture0);
    }

    void BufferOutlineMaterial::setOutlineColor(Color color) {
        this->outlineColor = color;
    }

    void BufferOutlineMaterial::setOutlineSize(UInt32 outlineSize) {
        this->outlineSize = outlineSize;
    }

}
