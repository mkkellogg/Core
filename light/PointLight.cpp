#include "PointLight.h"
#include "../Engine.h"
#include "../Graphics.h"
#include "../render/RenderTargetCube.h"

namespace Core {

    const Real PointLight::NearPlane = 0.1f;
    const Real PointLight::FarPlane = 100.0f;

    PointLight::PointLight(WeakPointer<Object3D> owner, Bool shadowsEnabled, 
                           UInt32 shadowMapSize, Real constantShadowBias, Real angularShadowBias): 
        ShadowLight(owner, LightType::Point, shadowsEnabled, shadowMapSize, constantShadowBias, angularShadowBias) {
        this->attenuationOverride = false;
        this->attenuation = 1.0f;
        this->radius = 1.0f;
    }

    PointLight::~PointLight() {
        
    }

    void PointLight::init() {
        if (this->shadowsEnabled) {
            this->buildShadowMap();
        }
    }

    void PointLight::setShadowsEnabled(Bool enabled) {
        ShadowLight::setShadowsEnabled(enabled);
        if (enabled && !this->shadowMap.isValid()) {
            this->buildShadowMap();
        }
    }

    WeakPointer<RenderTarget> PointLight::getShadowMap() {
        return this->shadowMap;
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

    void PointLight::buildShadowMap() {
        TextureAttributes colorTextureAttributes;
        colorTextureAttributes.Format = TextureFormat::R32F;
        colorTextureAttributes.FilterMode = TextureFilter::Linear;
        TextureAttributes depthTextureAttributes;
        Vector2u renderTargetSize(this->shadowMapSize, this->shadowMapSize);
        this->shadowMap = Engine::instance()->getGraphicsSystem()->createRenderTargetCube(true, true, false, colorTextureAttributes,
                                                                                          depthTextureAttributes, renderTargetSize);
    }
}