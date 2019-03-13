#include "AmbientIBLLight.h"
#include "../Engine.h"
#include "../render/RenderTargetCube.h"

namespace Core {

    AmbientIBLLight::AmbientIBLLight(WeakPointer<Object3D> owner): Light(owner, LightType::AmbientIBL) {
        
    }

    AmbientIBLLight::~AmbientIBLLight() {
        
    }

    void AmbientIBLLight::init() {
       
    }

    void AmbientIBLLight::setIBLTexture(WeakPointer<CubeTexture> texture) {
        this->iblTexture = texture;
    }

    WeakPointer<CubeTexture> AmbientIBLLight::getIBLTexture() {
        return this->iblTexture;
    }

}