#include "SpecularIBLPreFilteredRendererMaterial.h"
#include "../material/Shader.h"
#include "../util/WeakPointer.h"
#include "StandardAttributes.h"
#include "StandardUniforms.h"
#include "../Engine.h"
#include "../material/ShaderManager.h"

namespace Core {

    SpecularIBLPreFilteredRendererMaterial::SpecularIBLPreFilteredRendererMaterial(WeakPointer<Graphics> graphics) : SkyboxMaterial("SpecularIBLPreFilteredRenderer", graphics) {
        this->roughnessLocation = -1;
        this->textureResolutionLocation = -1;
        this->setPhysical(true);
        this->roughness = 0.5;
        this->textureResolution = 512;
    }

    SpecularIBLPreFilteredRendererMaterial::~SpecularIBLPreFilteredRendererMaterial() {
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
        WeakPointer<SpecularIBLPreFilteredRendererMaterial> specularIBLPreFilteredRenderMat = WeakPointer<Material>::dynamicPointerCast<SpecularIBLPreFilteredRendererMaterial>(target);
        if(specularIBLPreFilteredRenderMat.isValid()) {
            SkyboxMaterial::copyTo(target);
            specularIBLPreFilteredRenderMat->roughness = this->roughness;
            specularIBLPreFilteredRenderMat->roughnessLocation = this->roughnessLocation;
            specularIBLPreFilteredRenderMat->textureResolution = this->textureResolution;
            specularIBLPreFilteredRenderMat->textureResolutionLocation = this->textureResolutionLocation;
        } else {
           throw InvalidArgumentException("SpecularIBLPreFilteredRendererMaterial::copyTo() -> 'target must be same material."); 
        }
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
