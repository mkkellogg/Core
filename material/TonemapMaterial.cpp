#include "TonemapMaterial.h"
#include "../material/Shader.h"
#include "../util/WeakPointer.h"
#include "StandardAttributes.h"
#include "StandardUniforms.h"
#include "../Engine.h"
#include "../material/ShaderManager.h"

namespace Core {

    TonemapMaterial::TonemapMaterial(const std::string& vertShaderName, const std::string& fragShaderName, WeakPointer<Graphics> graphics): ShaderMaterial(vertShaderName, fragShaderName, graphics) {
        this->toneMapType = ToneMapType::Reinhard; 
        this->exposure = 1.0f;
        this->gamma = 2.2f;
    }
    
    TonemapMaterial::TonemapMaterial(WeakPointer<Graphics> graphics) : TonemapMaterial("Tonemap", "Tonemap", graphics) {
    }

    Bool TonemapMaterial::build() {
        if(!ShaderMaterial::build()) return false;
        this->bindShaderVarLocations();
        return true;
    }

    Int32 TonemapMaterial::getShaderLocation(StandardAttribute attribute, UInt32 offset) {
        switch (attribute) {
            case StandardAttribute::Position:
                return this->positionLocation;
            case StandardAttribute::Color:
                return this->colorLocation;
            default:
                return -1;
        }
    }

    Int32 TonemapMaterial::getShaderLocation(StandardUniform uniform, UInt32 offset) {
        switch (uniform) {
            case StandardUniform::ProjectionMatrix:
                return this->projectionMatrixLocation;
            case StandardUniform::ViewMatrix:
                return this->viewMatrixLocation;
            case StandardUniform::ModelMatrix:
                return this->modelMatrixLocation;
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

    void TonemapMaterial::copyTo(WeakPointer<Material> target) {
        ShaderMaterial::copyTo(target);
        WeakPointer<TonemapMaterial> _target = WeakPointer<Material>::dynamicPointerCast<TonemapMaterial>(target);
        _target->positionLocation = this->positionLocation;
        _target->colorLocation = this->colorLocation;
        _target->projectionMatrixLocation = this->projectionMatrixLocation;
        _target->viewMatrixLocation = this->viewMatrixLocation;
        _target->modelMatrixLocation = this->modelMatrixLocation;
        _target->textureLocation = this->textureLocation;
        _target->depthTextureLocation = this->depthTextureLocation;
        _target->exposureLocation = this->exposureLocation;
        _target->gammaLocation = this->gammaLocation;
        _target->toneMapTypeLocation = this->toneMapTypeLocation;
        _target->texture = this->texture;
        _target->depthTexture = this->depthTexture;
        _target->exposure = this->exposure;
        _target->gamma = this->gamma;
        _target->toneMapType = this->toneMapType;
    }

    WeakPointer<Material> TonemapMaterial::clone() {
        WeakPointer<TonemapMaterial> newMaterial = Engine::instance()->createMaterial<TonemapMaterial>(false);
        this->copyTo(newMaterial);
        return newMaterial;
    }

    void TonemapMaterial::bindShaderVarLocations() {
        this->positionLocation = this->shader->getAttributeLocation(StandardAttribute::Position);
        this->colorLocation = this->shader->getAttributeLocation(StandardAttribute::Color);
        this->textureLocation = this->shader->getUniformLocation(StandardUniform::Texture0);
        this->depthTextureLocation = this->shader->getUniformLocation(StandardUniform::DepthTexture);
        this->projectionMatrixLocation = this->shader->getUniformLocation(StandardUniform::ProjectionMatrix);
        this->viewMatrixLocation = this->shader->getUniformLocation(StandardUniform::ViewMatrix);
        this->modelMatrixLocation = this->shader->getUniformLocation(StandardUniform::ModelMatrix);
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
