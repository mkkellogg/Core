#include "SpecularIBLPreFilteredRendererMaterial.h"
#include "../material/Shader.h"
#include "../util/WeakPointer.h"
#include "StandardAttributes.h"
#include "StandardUniforms.h"
#include "../Engine.h"
#include "../material/ShaderManager.h"

namespace Core {

    SpecularIBLPreFilteredRendererMaterial::SpecularIBLPreFilteredRendererMaterial(WeakPointer<Graphics> graphics) : SkyboxMaterial("SpecularIBLPreFilteredRenderer", graphics) {
        this->setPhysical(true);
        this->roughness = 0.5;
        this->textureResolution = 512;
    }

    void SpecularIBLPreFilteredRendererMaterial::sendCustomUniformsToShader() {
        SkyboxMaterial::sendCustomUniformsToShader();
        this->shader->setUniform1f(this->roughnessLocation, this->roughness);
        this->shader->setUniform1f(this->textureResolutionLocation, this->textureResolution);
    }

    void SpecularIBLPreFilteredRendererMaterial::setRoughness(Real roughness) {
        this->roughness = roughness;
    }

    void SpecularIBLPreFilteredRendererMaterial::setTextureResolution(UInt32 resolution) {
        this->textureResolution = resolution;
    }

    void SpecularIBLPreFilteredRendererMaterial::copyTo(WeakPointer<Material> target) {
        SkyboxMaterial::copyTo(target);
        WeakPointer<SpecularIBLPreFilteredRendererMaterial> _target = WeakPointer<Material>::dynamicPointerCast<SpecularIBLPreFilteredRendererMaterial>(target);
        _target->roughness = this->roughness;
        _target->roughnessLocation = this->roughnessLocation;
        _target->textureResolution = this->textureResolution;
        _target->textureResolutionLocation = this->textureResolutionLocation;
    }

    WeakPointer<Material> SpecularIBLPreFilteredRendererMaterial::clone() {
        WeakPointer<SpecularIBLPreFilteredRendererMaterial> newMaterial = Engine::instance()->createMaterial<SpecularIBLPreFilteredRendererMaterial>(false);
        this->copyTo(newMaterial);
        return newMaterial;
    }

    void SpecularIBLPreFilteredRendererMaterial::bindShaderVarLocations() {
        SkyboxMaterial::bindShaderVarLocations();
        this->roughnessLocation = this->shader->getUniformLocation("roughness");
        this->textureResolutionLocation = this->shader->getUniformLocation("cubeTextureResolution");
    }
}
