#include "PointLight.h"

namespace Core {

    PointLight::PointLight(WeakPointer<Object3D> owner): Light(owner) {
        this->attenuationOverride = false;
        this->attenuation = 1.0f;
        this->radius = 1.0f;
    }

    void PointLight::setAttenuation(Real attenuation) {
        this->attenuation = attenuation;
        attenuationOverride = true;
    }

    Real PointLight::setAttenuation() const {
        return this->attenuation;
    }

    Real PointLight::getRadius() const {
        return this->radius;
    }

    void PointLight::setRadius(Real radius) {
        this->radius = radius;
        this->calcAttentuationForCurrentRadius();
    }

    void PointLight::calcAttentuationForCurrentRadius() {
        if (!this->attenuationOverride) {
            // multiplying by 0.95f causes the light to fully attenuate slightly before reaching maximum range
            this->attenuation = 1.0f / (this->radius * 0.95f);
        }
    }
}