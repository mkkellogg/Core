#include "AmbientIBLLight.h"
#include "../Engine.h"
#include "../render/RenderTargetCube.h"
#include "../render/RenderTarget2D.h"
#include "../render/ReflectionProbe.h"
#include "../image/Texture.h"
#include "../image/CubeTexture.h"
#include "../image/Texture2D.h"

namespace Core {

    AmbientIBLLight::AmbientIBLLight(WeakPointer<Object3D> owner): Light(owner, LightType::AmbientIBL) {
    }

    AmbientIBLLight::~AmbientIBLLight() {
        
    }

    void AmbientIBLLight::init() {
       
    }

    void AmbientIBLLight::setIrradianceMap(WeakPointer<CubeTexture> irradianceMap) {
        this->irradianceMap = irradianceMap;
    }

    WeakPointer<CubeTexture> AmbientIBLLight::getIrradianceMap() {
        return this->irradianceMap;
    }

    void AmbientIBLLight::setSpecularIBLPreFilteredMap(WeakPointer<CubeTexture> specularIBLPreFilteredMap) {
        this->specularIBLPreFilteredMap = specularIBLPreFilteredMap;
    }

    WeakPointer<CubeTexture> AmbientIBLLight::getSpecularIBLPreFilteredMap() {
        return this->specularIBLPreFilteredMap;
    }

    void AmbientIBLLight::setSpecularIBLBRDFMap(WeakPointer<Texture2D> specularIBLBRDFMap) {
        this->specularIBLBRDFMap = specularIBLBRDFMap;
    }

    WeakPointer<Texture2D> AmbientIBLLight::getSpecularIBLBRDFMap() {
        return this->specularIBLBRDFMap;
    }

    void AmbientIBLLight::setReflectionProbe(WeakPointer<ReflectionProbe> reflectionProbe) {
        this->reflectionProbe = reflectionProbe;
    }

    void AmbientIBLLight::updateMapsFromReflectionProbe() {
        if (this->reflectionProbe.isValid()) {
            this->setIrradianceMap(this->reflectionProbe->getIrradianceMap());
            this->setSpecularIBLPreFilteredMap(this->reflectionProbe->getSpecularIBLPreFilteredMap());
            this->setSpecularIBLBRDFMap(this->reflectionProbe->getSpecularIBLBRDFMap());
        }
    }
}