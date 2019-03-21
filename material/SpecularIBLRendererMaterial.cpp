#include "SpecularIBLRendererMaterial.h"
#include "../material/Shader.h"
#include "../util/WeakPointer.h"
#include "StandardAttributes.h"
#include "StandardUniforms.h"
#include "../Engine.h"
#include "../material/ShaderManager.h"

namespace Core {

    SpecularIBLRendererMaterial::SpecularIBLRendererMaterial(WeakPointer<Graphics> graphics) : SkyboxMaterial("SpecularIBLRenderer", "SpecularIBLRenderer", graphics) {
        this->setPhysical(true);
        this->roughness = 0.5;
    }

    void SpecularIBLRendererMaterial::sendCustomUniformsToShader() {
        SkyboxMaterial::sendCustomUniformsToShader();
        this->shader->setUniform1f(this->roughnessLocation, this->roughness);
    }

    void SpecularIBLRendererMaterial::setRoughness(Real roughness) {
        this->roughness = roughness;
    }

     void SpecularIBLRendererMaterial::copyTo(WeakPointer<Material> target) {
        SkyboxMaterial::copyTo(target);
        WeakPointer<SpecularIBLRendererMaterial> _target = WeakPointer<Material>::dynamicPointerCast<SpecularIBLRendererMaterial>(target);
        _target->roughness = this->roughness;
        _target->roughnessLocation = this->roughnessLocation;
    }

    WeakPointer<Material> SpecularIBLRendererMaterial::clone() {
        WeakPointer<SpecularIBLRendererMaterial> newMaterial = Engine::instance()->createMaterial<SpecularIBLRendererMaterial>(false);
        this->copyTo(newMaterial);
        return newMaterial;
    }

    void SpecularIBLRendererMaterial::bindShaderVarLocations() {
        SkyboxMaterial::bindShaderVarLocations();
        this->roughnessLocation = this->shader->getUniformLocation("roughness");
    }
}
