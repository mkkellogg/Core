#include "EquirectangularMaterial.h"
#include "../material/Shader.h"
#include "../Engine.h"
#include "../Graphics.h"
#include "../image/Texture2D.h"
#include "../material/ShaderManager.h"

namespace Core {

    EquirectangularMaterial::EquirectangularMaterial() {
    }

    EquirectangularMaterial::~EquirectangularMaterial() {
        if (this->texture.isValid()) Graphics::safeReleaseObject(this->texture);
    }

    Bool EquirectangularMaterial::build() {
        WeakPointer<Graphics> graphics = Engine::instance()->getGraphicsSystem();
        ShaderManager& shaderManager = graphics->getShaderManager();
        this->shader = shaderManager.getShader("Equirectangular");
        graphics->activateShader(this->shader);
        this->bindShaderVarLocations();
        return true;
    }

    void EquirectangularMaterial::setTexture(WeakPointer<Texture2D> texture) {
        this->texture = texture;
    }

    void EquirectangularMaterial::sendCustomUniformsToShader() {
        if (this->texture) {
            this->shader->setTexture2D(0, this->textureLocation, this->texture->getTextureID());
        }
    }

    void EquirectangularMaterial::copyTo(WeakPointer<Material> target) {
        WeakPointer<EquirectangularMaterial> equirectangularMaterial = WeakPointer<Material>::dynamicPointerCast<EquirectangularMaterial>(target);
        if (equirectangularMaterial.isValid()) {
            BaseMaterial::copyTo(target);
            equirectangularMaterial->textureLocation = this->textureLocation;
        } else {
            throw InvalidArgumentException("EquirectangularMaterial::copyTo() -> 'target must be same material.");
        }
    }

    WeakPointer<Material> EquirectangularMaterial::clone() {
        WeakPointer<EquirectangularMaterial> newMaterial = Engine::instance()->createMaterial<EquirectangularMaterial>(false);
        this->copyTo(newMaterial);
        return newMaterial;
    }

    void EquirectangularMaterial::bindShaderVarLocations() {
        BaseMaterial::bindShaderVarLocations();
        this->textureLocation = this->shader->getUniformLocation("equirectangularTexture");
    }

    UInt32 EquirectangularMaterial::textureCount() {
        if (this->texture) {
            return 1;
        }
        else return 0;
    }
}
