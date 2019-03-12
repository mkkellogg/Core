#include "PhysicalSkyboxMaterial.h"
#include "../material/Shader.h"
#include "../util/WeakPointer.h"
#include "StandardAttributes.h"
#include "StandardUniforms.h"
#include "../Engine.h"
#include "../material/ShaderManager.h"

namespace Core {

    PhysicalSkyboxMaterial::PhysicalSkyboxMaterial(WeakPointer<Graphics> graphics) : SkyboxMaterial("PhysicalSkybox", "PhysicalSkybox", graphics) {
        this->exposure = 1.0f;
        this->setPhysical(true);
    }
    
    void PhysicalSkyboxMaterial::sendCustomUniformsToShader() {
        SkyboxMaterial::sendCustomUniformsToShader();
        this->shader->setUniform1f(this->exposureLocation, this->exposure);
    }

    void PhysicalSkyboxMaterial::setExposure(Real exposure) {
        this->exposure = exposure;
    }

    void PhysicalSkyboxMaterial::copyTo(WeakPointer<Material> target) {
        SkyboxMaterial::copyTo(target);
        WeakPointer<PhysicalSkyboxMaterial> _target = WeakPointer<Material>::dynamicPointerCast<PhysicalSkyboxMaterial>(target);
        _target->exposureLocation = this->exposureLocation;
    }

    WeakPointer<Material> PhysicalSkyboxMaterial::clone() {
        WeakPointer<PhysicalSkyboxMaterial> newMaterial = Engine::instance()->createMaterial<PhysicalSkyboxMaterial>(false);
        this->copyTo(newMaterial);
        newMaterial->exposureLocation = this->exposureLocation;
        return newMaterial;
    }

    void PhysicalSkyboxMaterial::bindShaderVarLocations() {
        SkyboxMaterial::bindShaderVarLocations();
        this->exposureLocation = this->shader->getUniformLocation("exposure");
    }
}
