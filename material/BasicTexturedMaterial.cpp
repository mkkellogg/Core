#include "BasicTexturedMaterial.h"
#include "../material/Shader.h"
#include "../util/WeakPointer.h"
#include "StandardAttributes.h"
#include "StandardUniforms.h"
#include "../image/Texture.h"
#include "../Engine.h"
#include "../Graphics.h"
#include "../material/ShaderManager.h"

namespace Core {

    BasicTexturedMaterial::BasicTexturedMaterial(WeakPointer<Graphics> graphics): BaseMaterial(graphics) {
        this->uvLocation = -1;
    }

    BasicTexturedMaterial::~BasicTexturedMaterial() {
        if (this->texture.isValid()) Graphics::safeReleaseObject(this->texture);
    }

    Bool BasicTexturedMaterial::build() {
        WeakPointer<Graphics> graphics = Engine::instance()->getGraphicsSystem();
        ShaderManager& shaderManager = graphics->getShaderManager();
        this->shader = shaderManager.getShader("BasicTextured");
        graphics->activateShader(this->shader);
        this->bindShaderVarLocations();
        return true;
    }

    Int32 BasicTexturedMaterial::getShaderLocation(StandardAttribute attribute, UInt32 offset) {
        Int32 attributeLocation = BaseMaterial::getShaderLocation(attribute, offset);
        if (attributeLocation >= 0) return attributeLocation;
        switch (attribute) {
            case StandardAttribute::AlbedoUV:
                return this->uvLocation;
            default:
                return -1;
        }
    }

    void BasicTexturedMaterial::setTexture(WeakPointer<Texture> texture) {
        this->texture = texture;
    }

    void BasicTexturedMaterial::sendCustomUniformsToShader() {
        if (this->texture) {
            this->shader->setTexture2D(0, textureLocation, this->texture->getTextureID());
        }
    }

    void BasicTexturedMaterial::copyTo(WeakPointer<Material> target) {
        WeakPointer<BasicTexturedMaterial> basicTexturedMaterial = WeakPointer<Material>::dynamicPointerCast<BasicTexturedMaterial>(target);
        if (basicTexturedMaterial.isValid()) {
            BaseMaterial::copyTo(target);
            basicTexturedMaterial->texture = this->texture;
            basicTexturedMaterial->textureLocation = this->textureLocation;
            basicTexturedMaterial->uvLocation = this->uvLocation;
        } else {
            throw InvalidArgumentException("BasicTexturedMaterial::copyTo() -> 'target must be same material.");
        }
    }

    WeakPointer<Material> BasicTexturedMaterial::clone() {
        WeakPointer<BasicTexturedMaterial> newMaterial = Engine::instance()->createMaterial<BasicTexturedMaterial>(false);
        this->copyTo(newMaterial);
        return newMaterial;
    }

    void BasicTexturedMaterial::bindShaderVarLocations() {
        BaseMaterial::bindShaderVarLocations();
        this->uvLocation = this->shader->getAttributeLocation(StandardAttribute::AlbedoUV);
        this->textureLocation = this->shader->getUniformLocation("twoDtexture");
    }
}