#include "ShadowLight.h"

namespace Core {

    ShadowLight::ShadowLight(WeakPointer<Object3D> owner, LightType type, Bool shadowsEnabled, 
                            UInt32 shadowMapSize, Real constantShadowBias, Real angularShadowBias): 
        Light(owner, type), shadowsEnabled(shadowsEnabled), shadowMapSize(shadowMapSize), constantShadowBias(constantShadowBias), angularShadowBias(angularShadowBias) {
            this->shadowSoftness = Softness::Hard;
            this->faceCullingEnabled = true;
            this->cullFace = RenderState::CullFace::Back;
    }

    ShadowLight::~ShadowLight() {

    }

    void ShadowLight::setShadowsEnabled(Bool enabled) {
        this->shadowsEnabled = enabled;
    }

    Bool ShadowLight::getShadowsEnabled() const {
        return this->shadowsEnabled;
    }

    void ShadowLight::setConstantShadowBias(Real bias) {
        this->constantShadowBias = bias;
    }

    Real ShadowLight::getConstantShadowBias() const {
        return this->constantShadowBias;
    }

    void ShadowLight::setAngularShadowBias(Real bias) {
        this->angularShadowBias = bias;
    }

    Real ShadowLight::getAngularShadowBias() const {
        return this->angularShadowBias;
    }

    UInt32 ShadowLight::getShadowMapSize() const {
        return this->shadowMapSize;
    }

    void ShadowLight::setShadowSoftness(Softness softness) {
        this->shadowSoftness = softness;
    }

    ShadowLight::Softness ShadowLight::getShadowSoftness() const {
        return this->shadowSoftness;
    }

    Bool ShadowLight::getFaceCullingEnabled() const {
        return this->faceCullingEnabled;
    }

    void ShadowLight::setFaceCullingEnabled(Bool enabled) {
        this->faceCullingEnabled = enabled;
    }

    RenderState::CullFace ShadowLight::getCullFace() {
        return this->cullFace;
    }

    void ShadowLight::setCullFace(RenderState::CullFace cullFace) {
        this->cullFace = cullFace;
    }
}