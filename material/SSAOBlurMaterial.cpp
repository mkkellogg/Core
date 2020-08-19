#include "SSAOBlurMaterial.h"
#include "../material/Shader.h"
#include "../Engine.h"
#include "../Graphics.h"
#include "../material/ShaderManager.h"
#include "../image/Texture.h"

namespace Core {

    SSAOBlurMaterial::SSAOBlurMaterial(WeakPointer<Graphics> graphics) : BaseMaterial(graphics) {
    }

    SSAOBlurMaterial::~SSAOBlurMaterial() {
    }
    
    Bool SSAOBlurMaterial::build() {
        WeakPointer<Graphics> graphics = Engine::instance()->getGraphicsSystem();
        ShaderManager& shaderManager = graphics->getShaderManager();
        this->shader = shaderManager.getShader("SSAOBlur");
        graphics->activateShader(this->shader);
        this->bindShaderVarLocations();
        return true;
    }

    void SSAOBlurMaterial::sendCustomUniformsToShader() {
        UInt32 textureLoc = 0;
        this->shader->setTexture2D(textureLoc, this->ssaoInput->getTextureID());
        this->shader->setUniform1i(this->ssaoInputLocation, textureLoc);
        textureLoc++;
    }

    WeakPointer<Material> SSAOBlurMaterial::clone() {
        WeakPointer<SSAOBlurMaterial> newMaterial = Engine::instance()->createMaterial<SSAOBlurMaterial>(false);
        this->copyTo(newMaterial);
        return newMaterial;
    }

     void SSAOBlurMaterial::copyTo(WeakPointer<Material> target) {
        WeakPointer<SSAOBlurMaterial> ssaoMaterial = WeakPointer<Material>::dynamicPointerCast<SSAOBlurMaterial>(target);
        if (ssaoMaterial.isValid()) {
            BaseMaterial::copyTo(target);
            ssaoMaterial->ssaoInputLocation = this->ssaoInputLocation;
            ssaoMaterial->ssaoInput = this->ssaoInput;
        } else {
            throw InvalidArgumentException("SSAOBlurMaterial::copyTo() -> 'target must be same material.");
        }
    }

    void SSAOBlurMaterial::bindShaderVarLocations() {
        BaseMaterial::bindShaderVarLocations();
        this->ssaoInputLocation = this->shader->getUniformLocation("ssaoInput");
    }

    void SSAOBlurMaterial::setSSAOInput(WeakPointer<Texture> ssaoInput) {
        this->ssaoInput = ssaoInput;
    }

}
