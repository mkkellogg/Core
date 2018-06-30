#include "PointLight.h"

namespace Core {

    PointLight::PointLight(WeakPointer<Object3D> owner): Light(owner) {
        this->attenuationOverride = false;
        this->attenuation = 1.0f;
        this->radius = 1.0f;
    }

    PointLight::~PointLight() {

    }

    void PointLight::setAttenuation(Real attenuation) {
        this->attenuation = attenuation;
        this->attenuationOverride = true;
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

    void PointLight::setPosition(const Point3<Real>& postion) {
        this->setPosition(postion.x, postion.y, postion.z);
    }

    void PointLight::setPosition(Real x, Real y, Real z) {
        this->position.set(x, y, z);
    }

    Point3r PointLight::getPosition() {
        return this->position;
    }

    void PointLight::calcAttentuationForCurrentRadius() {
        if (!this->attenuationOverride) {
            // multiplying by 0.95f causes the light to fully attenuate slightly before reaching maximum range
            this->attenuation = 1.0f / (this->radius * 0.95f);
        }
    }
}