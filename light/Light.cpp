#include "Light.h"

namespace Core {

    Light::Light(LightType type, WeakPointer<Object3D> owner): Object3DComponent(owner), type(type) {
        this->color.set(1, 1, 1, 1);
        this->intensity = 1;
        this->shadowsEnabled = false;
        this->cullingMask = IntMaskUtil::createMask();
        this->shadowBias = 0.03f;
        this->shadowMapSize = 1024;
    }

    Light::~Light() {
        
    }

    WeakPointer<RenderTarget> Light::getShadowMap() {
        return WeakPointer<RenderTarget>();
    }

    const Color& Light::getColor() const {
        return this->color;
    }

    void Light::setColor(const Color& color) {
        this->color = color;
    }

    void Light::setColor(Real r, Real g, Real b, Real a) {
        this->color.set(r, g, b, a);
    }

    void Light::setIntensity(Real intensity) {
        this->intensity = intensity;
    }

    Real Light::getIntensity() const {
        return this->intensity;
    }

      void Light::setShadowsEnabled(Bool enabled) {
        this->shadowsEnabled = enabled;
    }

    Bool Light::getShadowsEnabled() const {
        return this->shadowsEnabled;
    }

    void Light::setCullingMask(IntMask mask) {
       this->cullingMask = mask;
    }

    IntMask Light::getCullingMask() const {
        return this->cullingMask;
    }

    LightType Light::getType() const {
        return this->type;
    }

    void Light::setShadowBias(Real bias) {
        this->shadowBias = bias;
    }

    Real Light::getShadowBias() const {
        return this->shadowBias;
    }

    void Light::setShadowMapSize(UInt32 size) {
        this->shadowMapSize = size;
    }

    UInt32 Light::getShadowMapSize() const {
        return this->shadowMapSize;
    }
}