#include "TonemapMaterial.h"
#include "../image/Texture2D.h"
#include "StandardUniforms.h"

namespace Core {

    TonemapMaterial::TonemapMaterial(const std::string& vertShaderName, const std::string& fragShaderName):
        ShaderMaterial<BaseMaterial>(vertShaderName, fragShaderName) {
            this->setInitialParams();
    }

    TonemapMaterial::TonemapMaterial(const std::string& builtInShaderName):
        ShaderMaterial<BaseMaterial>(builtInShaderName) {
            this->setInitialParams();
    }
    
    TonemapMaterial::TonemapMaterial() : TonemapMaterial("Tonemap") {
    }

    TonemapMaterial::~TonemapMaterial() {
        if (this->texture.isValid()) Graphics::safeReleaseObject(this->texture);
        if (this->depthTexture.isValid()) Graphics::safeReleaseObject(this->depthTexture);
    }

    void TonemapMaterial::setInitialParams() {

        this->textureLocation = -1;
        this->depthTextureLocation = -1;
        this->exposureLocation = -1;
        this->gammaLocation = -1;
        this->toneMapTypeLocation = -1;

        this->toneMapType = ToneMapType::Reinhard; 
        this->exposure = 1.0f;
        this->gamma = 2.2f;
    }

    void TonemapMaterial::setTexture(WeakPointer<Texture2D> texture) {
        this->texture = texture;
    }

    void TonemapMaterial::setDepthTexture(WeakPointer<Texture2D> texture) {
        this->depthTexture = depthTexture;
    }

    void TonemapMaterial::setExposure(Real exposure) {
        this->exposure = exposure;
    }

    void TonemapMaterial::setGamma(Real gamma) {
        this->gamma = gamma;
    }

    void TonemapMaterial::setToneMapType(ToneMapType type) {
        this->toneMapType = type;
    }

    Int32 TonemapMaterial::getShaderLocation(StandardUniform uniform, UInt32 offset) {
        Int32 uniformLocation = BaseMaterial::getShaderLocation(uniform, offset);
        if (uniformLocation >= 0) return uniformLocation;
        switch (uniform) {
            case StandardUniform::Texture0:
                return this->textureLocation;
            case StandardUniform::DepthTexture:
                return this->depthTextureLocation;
            default:
                return -1;
        }
    }

    void TonemapMaterial::sendCustomUniformsToShader() {
        this->shader->setUniform1f(this->exposureLocation, this->exposure);
        this->shader->setUniform1f(this->gammaLocation, this->gamma);
        this->shader->setUniform1i(this->toneMapTypeLocation, (Int32)this->toneMapType);
    }

    void TonemapMaterial::copyTo(WeakPointer<Material> target) {
        WeakPointer<TonemapMaterial> tonemapMaterial = WeakPointer<Material>::dynamicPointerCast<TonemapMaterial>(target);
        if (tonemapMaterial.isValid()) {
            BaseMaterial::copyTo(target);
            tonemapMaterial->textureLocation = this->textureLocation;
            tonemapMaterial->depthTextureLocation = this->depthTextureLocation;
            tonemapMaterial->exposureLocation = this->exposureLocation;
            tonemapMaterial->gammaLocation = this->gammaLocation;
            tonemapMaterial->toneMapTypeLocation = this->toneMapTypeLocation;
            
            tonemapMaterial->texture = this->texture;
            tonemapMaterial->depthTexture = this->depthTexture;
            tonemapMaterial->exposure = this->exposure;
            tonemapMaterial->gamma = this->gamma;
            tonemapMaterial->toneMapType = this->toneMapType;
        } else {
            throw InvalidArgumentException("TonemapMaterial::copyTo() -> 'target must be same material.");
        }
    }

    WeakPointer<Material> TonemapMaterial::clone() {
        WeakPointer<TonemapMaterial> newMaterial = Engine::instance()->createMaterial<TonemapMaterial>(false);
        this->copyTo(newMaterial);
        return newMaterial;
    }

    void TonemapMaterial::bindShaderVarLocations() {
        BaseMaterial::bindShaderVarLocations();
        this->textureLocation = this->shader->getUniformLocation(StandardUniform::Texture0);
        this->depthTextureLocation = this->shader->getUniformLocation(StandardUniform::DepthTexture);
        this->exposureLocation = this->shader->getUniformLocation("exposure");
        this->gammaLocation = this->shader->getUniformLocation("gamma");
        this->toneMapTypeLocation = this->shader->getUniformLocation("toneMapType");
    }

    UInt32 TonemapMaterial::textureCount() {
        UInt32 count = 0;
        if (this->texture) count++;
        if (this->depthTexture) count++;
        return count;
    }
}
