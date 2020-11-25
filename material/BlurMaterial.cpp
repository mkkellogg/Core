#include "BlurMaterial.h"
#include "../material/Shader.h"
#include "../Engine.h"
#include "../Graphics.h"
#include "../material/ShaderManager.h"
#include "../image/Texture.h"

namespace Core {

    BlurMaterial::BlurMaterial() {
        this->kernelSize = 2;
    }

    BlurMaterial::~BlurMaterial() {
    }
    
    Bool BlurMaterial::build() {
        WeakPointer<Graphics> graphics = Engine::instance()->getGraphicsSystem();
        ShaderManager& shaderManager = graphics->getShaderManager();
        this->shader = shaderManager.getShader("Blur");
        graphics->activateShader(this->shader);
        this->bindShaderVarLocations();
        return true;
    }

    Int32 BlurMaterial::getShaderLocation(StandardUniform uniform, UInt32 offset) {
        Int32 uniformLocation = BaseMaterial::getShaderLocation(uniform, offset);
        if (uniformLocation >= 0) return uniformLocation;
        switch (uniform) {
            case StandardUniform::Texture0:
                return this->inputLocation;
            default:
                return -1;
        }
    }

    void BlurMaterial::sendCustomUniformsToShader() {
        this->shader->setUniform1i(this->kernelSizeLocation, this->kernelSize);
    }

    WeakPointer<Material> BlurMaterial::clone() {
        WeakPointer<BlurMaterial> newMaterial = Engine::instance()->createMaterial<BlurMaterial>(false);
        this->copyTo(newMaterial);
        return newMaterial;
    }

     void BlurMaterial::copyTo(WeakPointer<Material> target) {
        WeakPointer<BlurMaterial> blurMaterial = WeakPointer<Material>::dynamicPointerCast<BlurMaterial>(target);
        if (blurMaterial.isValid()) {
            BaseMaterial::copyTo(target);
            blurMaterial->inputLocation = this->inputLocation;
            blurMaterial->kernelSizeLocation = this->kernelSizeLocation;
            blurMaterial->kernelSize = this->kernelSize;
        } else {
            throw InvalidArgumentException("BlurMaterial::copyTo() -> 'target must be same material.");
        }
    }

    void BlurMaterial::bindShaderVarLocations() {
        BaseMaterial::bindShaderVarLocations();
        this->inputLocation = this->shader->getUniformLocation(StandardUniform::Texture0);
        this->kernelSizeLocation = this->shader->getUniformLocation("kernelSize");
    }

     void BlurMaterial::setKernelSize(UInt32 kernelSize) {
         this->kernelSize = kernelSize;
     }

}
