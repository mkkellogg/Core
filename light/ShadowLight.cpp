#include "ShadowLight.h"

namespace Core {

    ShadowLight::ShadowLight(LightType type, WeakPointer<Object3D> owner): Light(type, owner) {
        this->shadowsEnabled = false;
        this->shadowBias = 0.03f;
        this->shadowMapSize = 1024;
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

    void ShadowLight::setShadowMapSize(UInt32 size) {
        this->shadowMapSize = size;
    }

    UInt32 ShadowLight::getShadowMapSize() const {
        return this->shadowMapSize;
    }
}