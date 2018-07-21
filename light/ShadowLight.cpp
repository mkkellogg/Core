#include "ShadowLight.h"

namespace Core {

    ShadowLight::ShadowLight(WeakPointer<Object3D> owner, LightType type, Bool shadowsEnabled, UInt32 shadowMapSize, Real shadowBias): 
        Light(owner, type), shadowsEnabled(shadowsEnabled), shadowMapSize(shadowMapSize), shadowBias(shadowBias) {
    }

    ShadowLight::~ShadowLight() {

    }

    void ShadowLight::setShadowsEnabled(Bool enabled) {
        this->shadowsEnabled = enabled;
    }

    Bool ShadowLight::getShadowsEnabled() const {
        return this->shadowsEnabled;
    }

    void ShadowLight::setShadowBias(Real bias) {
        this->shadowBias = bias;
    }

    Real ShadowLight::getShadowBias() const {
        return this->shadowBias;
    }

    UInt32 ShadowLight::getShadowMapSize() const {
        return this->shadowMapSize;
    }
}