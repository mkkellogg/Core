#include "SkyboxMaterial.h"
#include "../Graphics.h"
#include "../material/Shader.h"
#include "../util/WeakPointer.h"
#include "../Engine.h"
#include "../image/CubeTexture.h"
#include "../material/ShaderManager.h"

namespace Core {

    SkyboxMaterial::SkyboxMaterial(const std::string& vertShaderName, const std::string& fragShaderName, WeakPointer<Graphics> graphics):
        ShaderMaterial<BaseMaterial>(vertShaderName, fragShaderName, graphics) {
    }

    SkyboxMaterial::SkyboxMaterial(const std::string& builtInShaderName, WeakPointer<Graphics> graphics):
        ShaderMaterial<BaseMaterial>(builtInShaderName, graphics) {
    }
    
    SkyboxMaterial::SkyboxMaterial(WeakPointer<Graphics> graphics) : SkyboxMaterial("Skybox", graphics) {
    }

    SkyboxMaterial::~SkyboxMaterial() {
        if (this->texture.isValid()) Graphics::safeReleaseObject(this->texture);
    }
    
    void SkyboxMaterial::setTexture(WeakPointer<CubeTexture> texture) {
        this->texture = texture;
    }

    void SkyboxMaterial::sendCustomUniformsToShader() {
        this->shader->setTextureCube(0, this->texture->getTextureID());
        this->shader->setUniform1i(this->cubeTextureLocation, 0);
    }

    void SkyboxMaterial::copyTo(WeakPointer<Material> target) {
        WeakPointer<SkyboxMaterial> skyboxMaterial = WeakPointer<Material>::dynamicPointerCast<SkyboxMaterial>(target);
        if(skyboxMaterial.isValid()) {
            BaseMaterial::copyTo(skyboxMaterial);
            skyboxMaterial->cubeTextureLocation = this->cubeTextureLocation;
        } else {
            throw InvalidArgumentException("SkyboxMaterial::copyTo() -> 'target must be same material.");
        }
    }

    WeakPointer<Material> SkyboxMaterial::clone() {
        WeakPointer<SkyboxMaterial> newMaterial = Engine::instance()->createMaterial<SkyboxMaterial>(false);
        this->copyTo(newMaterial);
        return newMaterial;
    }

    void SkyboxMaterial::bindShaderVarLocations() {
        BaseMaterial::bindShaderVarLocations();
        this->cubeTextureLocation = this->shader->getUniformLocation("cubeTexture");
    }

    UInt32 SkyboxMaterial::textureCount() {
        return 1;
    }
}
