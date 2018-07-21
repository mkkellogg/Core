#include "DirectionalLight.h"
#include "../Engine.h"
#include "../render/RenderTarget2D.h"
#include "../common/Exception.h"

namespace Core {

    DirectionalLight::DirectionalLight(WeakPointer<Object3D> owner, UInt32 cascadeCount): ShadowLight(LightType::Directional, owner), cascadeCount(cascadeCount) {

    }

    DirectionalLight::~DirectionalLight() {
        
    }

    void DirectionalLight::init() {
        if (this->shadowsEnabled) {
            TextureAttributes colorTextureAttributes;
            colorTextureAttributes.Format = TextureFormat::R32F;
            colorTextureAttributes.FilterMode = TextureFilter::Linear;
            Vector2u renderTargetSize(this->shadowMapSize, this->shadowMapSize);
            for (UInt32 i = 0; i < this->cascadeCount; i++) {
                this->shadowMaps[i] = Engine::instance()->getGraphicsSystem()->createRenderTarget2D(true, true, false, colorTextureAttributes, renderTargetSize);
            }
        }
    }

    WeakPointer<RenderTarget> DirectionalLight::getShadowMap(UInt32 cascadeIndex) {
        if (cascadeIndex >= this-> cascadeCount) {
            throw OutOfRangeException("DirectionalLight::getShadowMap() -> 'cascadeIndex' is out of range.");
        }
        return this->shadowMaps[cascadeIndex];
    }

}