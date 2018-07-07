#include "PointLight.h"
#include "../Engine.h"

namespace Core {

    PointLight::PointLight(WeakPointer<Object3D> owner): Light(LightType::Point, owner) {
        this->attenuationOverride = false;
        this->attenuation = 1.0f;
        this->radius = 1.0f;
    }

    PointLight::~PointLight() {
        
    }

    void PointLight::init() {
       /* TextureAttributes textureAttributes;
        textureAttributes.Format = TextureFormat::R32F;
        textureAttributes.FilterMode = TextureFilter::Linear;
        textureAttributes.IsCube = treue;
        this->shadowMap = Engine::instance()->getGraphicsSystem()->createTexture2D(textureAttributes);*/
    }

    WeakPointer<Texture> PointLight::getShadowMap() {

    }

    void PointLight::setAttenuation(Real attenuation) {
        this->attenuation = attenuation;
        this->attenuationOverride = true;
    }

    Real PointLight::getAttenuation() const {
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