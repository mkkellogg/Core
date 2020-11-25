#include "BasicTexturedLitMaterial.h"
#include "../material/Shader.h"
#include "StandardAttributes.h"
#include "../image/Texture.h"
#include "../Engine.h"
#include "../Graphics.h"
#include "../material/ShaderManager.h"

namespace Core {

    BasicTexturedLitMaterial::BasicTexturedLitMaterial() {
        this->albedoMapEnabledLocation = -1;
        this->normalMapEnabledLocation = -1;
        this->albedoUVLocation = -1;
        this->albedoLocation = -1;
        this->albedoMapLocation = -1;
        this->normalUVLocation = -1;
        this->normalMapLocation = -1;
        this->albedoMapEnabled = false;
        this->normalMapEnabled = false;
    }

    BasicTexturedLitMaterial::~BasicTexturedLitMaterial() {
        if (this->albedoMap.isValid()) Graphics::safeReleaseObject(this->albedoMap);
        if (this->normalMap.isValid()) Graphics::safeReleaseObject(this->normalMap);
    }

    void BasicTexturedLitMaterial::setAlbedoMapEnabled(Bool enabled) {
        this->albedoMapEnabled = enabled;
    }

    void BasicTexturedLitMaterial::setAlbedoMap(WeakPointer<Texture> albedoMap) {
        this->albedoMap = albedoMap;
    }

    void BasicTexturedLitMaterial::setNormalMapEnabled(Bool enabled) {
        this->albedoMapEnabled = enabled;
    }

    void BasicTexturedLitMaterial::setNormalMap(WeakPointer<Texture> normalMap) {
        this->normalMap = normalMap;
    }

    Bool BasicTexturedLitMaterial::build() {
        WeakPointer<Graphics> graphics = Engine::instance()->getGraphicsSystem();
        ShaderManager& shaderManager = graphics->getShaderManager();
        this->shader = shaderManager.getShader("BasicTexturedLit");
        graphics->activateShader(this->shader);
        this->bindShaderVarLocations();
        this->setLit(true);
        return true;
    }

    Int32 BasicTexturedLitMaterial::getShaderLocation(StandardAttribute attribute, UInt32 offset) {
        Int32 attributeLocation = BaseMaterial::getShaderLocation(attribute, offset);
        if (attributeLocation >= 0) return attributeLocation;
        switch (attribute) {
            case StandardAttribute::AlbedoUV:
                return this->albedoUVLocation;
            default:
                return -1;
        }
    }

    void BasicTexturedLitMaterial::sendCustomUniformsToShader() {
        UInt32 samplerSlot = 0;
        if (this->albedoMapEnabled) {
            this->shader->setTexture2D(samplerSlot, this->albedoMapLocation, this->albedoMap->getTextureID());
            samplerSlot++;
        }
        else {
            this->shader->setUniform4f(this->albedoLocation, this->albedo.r, this->albedo.g, this->albedo.b, this->albedo.a);
        }
        if (this->normalMapEnabled) {
            this->shader->setTexture2D(samplerSlot, this->normalMapLocation, this->normalMap->getTextureID());
            samplerSlot++;
        }
        this->shader->setUniform1i(this->albedoMapEnabledLocation, this->albedoMapEnabled ? 1 : 0);
        this->shader->setUniform1i(this->normalMapEnabledLocation, this->normalMapEnabled ? 1 : 0);
    }

    void BasicTexturedLitMaterial::copyTo(WeakPointer<Material> target) {
        WeakPointer<BasicTexturedLitMaterial> basicTexturedLitMaterial = WeakPointer<Material>::dynamicPointerCast<BasicTexturedLitMaterial>(target);
        if (basicTexturedLitMaterial.isValid()) {
            BaseLitMaterial::copyTo(target);
            basicTexturedLitMaterial->albedoMap = this->albedoMap;
            basicTexturedLitMaterial->normalMap = this->normalMap;
            basicTexturedLitMaterial->albedoMapEnabled = this->albedoMapEnabled;
            basicTexturedLitMaterial->normalMapEnabled = this->normalMapEnabled;
            basicTexturedLitMaterial->albedoMapEnabledLocation = this->albedoMapEnabledLocation;
            basicTexturedLitMaterial->normalMapEnabledLocation = this->normalMapEnabledLocation;
            basicTexturedLitMaterial->albedoUVLocation = this->albedoUVLocation;
            basicTexturedLitMaterial->albedoLocation = this->albedoLocation;
            basicTexturedLitMaterial->albedoMapLocation = this->albedoMapLocation;
            basicTexturedLitMaterial->normalUVLocation = this->normalUVLocation;
            basicTexturedLitMaterial->normalMapLocation = this->normalMapLocation;
        } else {
            throw InvalidArgumentException("BasicTexturedLitMaterial::copyTo() -> 'target must be same material.");
        }
    }

    WeakPointer<Material> BasicTexturedLitMaterial::clone() {
        WeakPointer<BasicTexturedLitMaterial> newMaterial = Engine::instance()->createMaterial<BasicTexturedLitMaterial>(false);
        this->copyTo(newMaterial);
        return newMaterial;
    }

    void BasicTexturedLitMaterial::bindShaderVarLocations() {
        BaseLitMaterial::bindShaderVarLocations();
        this->albedoUVLocation = this->shader->getAttributeLocation(StandardAttribute::AlbedoUV);
        this->normalUVLocation = this->shader->getAttributeLocation(StandardAttribute::NormalUV);
        this->albedoMapEnabledLocation = this->shader->getUniformLocation("albedoMapEnabled");
        this->normalMapEnabledLocation = this->shader->getUniformLocation("normalMapEnabled");
        this->albedoLocation = this->shader->getUniformLocation("albedo");
        this->albedoMapLocation = this->shader->getUniformLocation("albedoMap");
        this->normalMapLocation = this->shader->getUniformLocation("normalMap");
    }

    UInt32 BasicTexturedLitMaterial::textureCount() {
        return 1;
    }
}