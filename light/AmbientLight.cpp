#include "AmbientLight.h"
#include "../Engine.h"
#include "../render/RenderTargetCube.h"

namespace Core {

    AmbientLight::AmbientLight(WeakPointer<Object3D> owner): Light(owner, LightType::Ambient) {
        
    }

    AmbientLight::~AmbientLight() {
        
    }

    void AmbientLight::init() {
       
    }

}