#include "BasicTexturedFullScreenQuadMaterial.h"
#include "../material/Shader.h"
#include "StandardAttributes.h"
#include "../image/Texture.h"
#include "../Engine.h"
#include "../Graphics.h"
#include "../material/ShaderManager.h"

namespace Core {

    BasicTexturedFullScreenQuadMaterial::BasicTexturedFullScreenQuadMaterial() {
        this->uvLocation = -1;
    }

    BasicTexturedFullScreenQuadMaterial::~BasicTexturedFullScreenQuadMaterial() {
        if (this->texture.isValid()) Graphics::safeReleaseObject(this->texture);
    }

    void BasicTexturedFullScreenQuadMaterial::setTexture(WeakPointer<Texture> texture) {
        this->texture = texture;
    }

    Bool BasicTexturedFullScreenQuadMaterial::build() {
        WeakPointer<Graphics> graphics = Engine::instance()->getGraphicsSystem();
        ShaderManager& shaderManager = graphics->getShaderManager();
        this->shader = shaderManager.getShader("BasicTexturedFullScreenQuad");
        graphics->activateShader(this->shader);
        this->bindShaderVarLocations();
        return true;
    }

    Int32 BasicTexturedFullScreenQuadMaterial::getShaderLocation(StandardAttribute attribute, UInt32 offset) {
        Int32 attributeLocation = BaseMaterial::getShaderLocation(attribute, offset);
        if (attributeLocation >= 0) return attributeLocation;
        switch (attribute) {
            case StandardAttribute::AlbedoUV:
                return this->uvLocation;
            default:
                return -1;
        }
    }

    void BasicTexturedFullScreenQuadMaterial::sendCustomUniformsToShader() {
        if (this->texture) {
            this->shader->setTexture2D(0, textureLocation, this->texture->getTextureID());
        }
    }

    void BasicTexturedFullScreenQuadMaterial::copyTo(WeakPointer<Material> target) {
        WeakPointer<BasicTexturedFullScreenQuadMaterial> basicTexturedMaterial = WeakPointer<Material>::dynamicPointerCast<BasicTexturedFullScreenQuadMaterial>(target);
        if (basicTexturedMaterial.isValid()) {
            BaseMaterial::copyTo(target);
            basicTexturedMaterial->texture = this->texture;
            basicTexturedMaterial->textureLocation = this->textureLocation;
            basicTexturedMaterial->uvLocation = this->uvLocation;
        } else {
            throw InvalidArgumentException("BasicTexturedFullScreenQuadMaterial::copyTo() -> 'target must be same material.");
        }
    }

    WeakPointer<Material> BasicTexturedFullScreenQuadMaterial::clone() {
        WeakPointer<BasicTexturedFullScreenQuadMaterial> newMaterial = Engine::instance()->createMaterial<BasicTexturedFullScreenQuadMaterial>(false);
        this->copyTo(newMaterial);
        return newMaterial;
    }

    void BasicTexturedFullScreenQuadMaterial::bindShaderVarLocations() {
        BaseMaterial::bindShaderVarLocations();
        this->uvLocation = this->shader->getAttributeLocation(StandardAttribute::AlbedoUV);
        this->textureLocation = this->shader->getUniformLocation("twoDtexture");
    }
}