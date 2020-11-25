#include "PhysicalSkyboxMaterial.h"

namespace Core {

    PhysicalSkyboxMaterial::PhysicalSkyboxMaterial() : SkyboxMaterial("PhysicalSkybox") {
        this->exposureLocation = -1;
        this->exposure = 1.0f;
        this->setPhysical(true);
    }

    void PhysicalSkyboxMaterial::setExposure(Real exposure) {
        this->exposure = exposure;
    }
    
    void PhysicalSkyboxMaterial::sendCustomUniformsToShader() {
        SkyboxMaterial::sendCustomUniformsToShader();
        this->shader->setUniform1f(this->exposureLocation, this->exposure);
    }

    void PhysicalSkyboxMaterial::copyTo(WeakPointer<Material> target) {
        WeakPointer<PhysicalSkyboxMaterial> physicalSkyboxMaterial = WeakPointer<Material>::dynamicPointerCast<PhysicalSkyboxMaterial>(target);
        if (physicalSkyboxMaterial.isValid()) {
            SkyboxMaterial::copyTo(target);
            physicalSkyboxMaterial->exposureLocation = this->exposureLocation;
            physicalSkyboxMaterial->exposure = this->exposure;
        } else {
            throw InvalidArgumentException("PhysicalSkyboxMaterial::copyTo() -> 'target must be same material.");
        }
    }

    WeakPointer<Material> PhysicalSkyboxMaterial::clone() {
        WeakPointer<PhysicalSkyboxMaterial> newMaterial = Engine::instance()->createMaterial<PhysicalSkyboxMaterial>(false);
        this->copyTo(newMaterial);
        return newMaterial;
    }

    void PhysicalSkyboxMaterial::bindShaderVarLocations() {
        SkyboxMaterial::bindShaderVarLocations();
        this->exposureLocation = this->shader->getUniformLocation("exposure");
    }
}
