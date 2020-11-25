#include "BaseLitMaterial.h"
#include "StandardUniforms.h"
#include "../Engine.h"
#include "../Graphics.h"
#include "../material/ShaderManager.h"

namespace Core {

    BaseLitMaterial::BaseLitMaterial() {
        for (UInt32 i = 0; i < Constants::MaxShaderLights; i++) {
            this->lightPositionLocation[i] = -1;
            this->lightDirectionLocation[i] = -1;
            for (UInt32 c = 0; c < Constants::MaxDirectionalCascades; c++) {
                UInt32 cascadeIndex = Constants::MaxDirectionalCascades * i + c;
                this->lightViewProjectionLocations[cascadeIndex] = -1;
                this->lightShadowMapLocations[cascadeIndex] = -1;
                this->lightCascadeEndLocations[cascadeIndex] = -1;
                this->lightShadowMapAspectLocations[cascadeIndex] = -1;
            }
            this->lightCascadeCountLocation[i] = -1;
            this->lightRangeLocation[i] = -1;
            this->lightTypeLocation[i] = -1;
            this->lightIntensityLocation[i] = -1;
            this->lightColorLocation[i] = -1;
            this->lightEnabledLocation[i] = -1;
            this->lightShadowsEnabledLocation[i] = -1;
            this->lightMatrixLocation[i] = -1;
            this->lightShadowCubeMapLocation[i] = -1;
            this->lightAngularShadowBiasLocation[i] = -1;
            this->lightConstantShadowBiasLocation[i] = -1;
            this->lightShadowMapSizeLocation[i] = -1;
            this->lightShadowSoftnessLocation[i] = -1;
            this->lightNearPlaneLocation[i] = -1;
        }
        this->lightCountLocation = -1;
    }

    BaseLitMaterial::~BaseLitMaterial(){
    }

    Int32 BaseLitMaterial::getShaderLocation(StandardUniform uniform, UInt32 offset) {
        Int32 baseUniform = BaseMaterial::getShaderLocation(uniform, offset);
        if (baseUniform >= 0) return baseUniform;

        if ((uniform == StandardUniform::LightViewProjection ||
             uniform == StandardUniform::LightShadowMap ||
             uniform == StandardUniform::LightCascadeEnd || 
             uniform == StandardUniform::LightShadowMapAspect) && 
             offset >= Constants::MaxDirectionalCascades * this->maxLightCount()) {
            throw InvalidArgumentException("BasicLitMaterial::getShaderLocation() -> invalid offset.");
        }

        switch (uniform) {
            case StandardUniform::LightPosition:
                return this->lightPositionLocation[offset];
            case StandardUniform::LightDirection:
                return this->lightDirectionLocation[offset];
            case StandardUniform::LightRange:
                return this->lightRangeLocation[offset];
            case StandardUniform::LightType:
                return this->lightTypeLocation[offset];
            case StandardUniform::LightIntensity:
                return this->lightIntensityLocation[offset];
            case StandardUniform::LightColor:
                return this->lightColorLocation[offset];
            case StandardUniform::LightEnabled:
                return this->lightEnabledLocation[offset];
            case StandardUniform::LightShadowsEnabled:
                return this->lightShadowsEnabledLocation[offset];
            case StandardUniform::LightMatrix:
                return this->lightMatrixLocation[offset];
            case StandardUniform::LightViewProjection:
                return this->lightViewProjectionLocations[offset];
            case StandardUniform::LightShadowMap:
                return this->lightShadowMapLocations[offset];
            case StandardUniform::LightCascadeEnd:
                return this->lightCascadeEndLocations[offset];
            case StandardUniform::LightShadowMapAspect:
                return this->lightShadowMapAspectLocations[offset];
            case StandardUniform::LightCascadeCount:
                return this->lightCascadeCountLocation[offset];
            case StandardUniform::LightShadowCubeMap:
                return this->lightShadowCubeMapLocation[offset];
            case StandardUniform::LightAngularShadowBias:
                return this->lightAngularShadowBiasLocation[offset];
            case StandardUniform::LightConstantShadowBias:
                return this->lightConstantShadowBiasLocation[offset];
            case StandardUniform::LightShadowMapSize:
                return this->lightShadowMapSizeLocation[offset];
            case StandardUniform::LightShadowSoftness:
                return this->lightShadowSoftnessLocation[offset];
            case StandardUniform::LightNearPlane:
                return this->lightNearPlaneLocation[offset];
            case StandardUniform::LightCount:
                return this->lightCountLocation;
            default:
                return -1;
        }
    }

    void BaseLitMaterial::copyTo(WeakPointer<Material> target) {
        WeakPointer<BaseLitMaterial> baseMaterial = WeakPointer<Material>::dynamicPointerCast<BaseLitMaterial>(target);
        if (baseMaterial.isValid()) {
            BaseMaterial::copyTo(target);
            for (UInt32 i = 0; i < Constants::MaxShaderLights; i++) {
                baseMaterial->lightPositionLocation[i] = this->lightPositionLocation[i];
                baseMaterial->lightDirectionLocation[i] = this->lightDirectionLocation[i];
                baseMaterial->lightRangeLocation[i] = this->lightRangeLocation[i];
                baseMaterial->lightTypeLocation[i] = this->lightTypeLocation[i];
                baseMaterial->lightIntensityLocation[i] = this->lightIntensityLocation[i];
                baseMaterial->lightColorLocation[i] = this->lightColorLocation[i];
                baseMaterial->lightEnabledLocation[i] = this->lightEnabledLocation[i];
                baseMaterial->lightShadowsEnabledLocation[i] = this->lightShadowsEnabledLocation[i];
                baseMaterial->lightMatrixLocation[i] = this->lightMatrixLocation[i];
                for (UInt32 c = 0; c < Constants::MaxDirectionalCascades; c++) {
                    UInt32 cascadeIndex = Constants::MaxDirectionalCascades * i + c;
                    baseMaterial->lightViewProjectionLocations[cascadeIndex] = this->lightViewProjectionLocations[cascadeIndex];
                    baseMaterial->lightShadowMapLocations[cascadeIndex] = this->lightShadowMapLocations[cascadeIndex];
                    baseMaterial->lightCascadeEndLocations[cascadeIndex] = this->lightCascadeEndLocations[cascadeIndex];
                    baseMaterial->lightShadowMapAspectLocations[cascadeIndex] = this->lightShadowMapAspectLocations[cascadeIndex];
                }
                baseMaterial->lightCascadeCountLocation[i] = this->lightCascadeCountLocation[i];
                baseMaterial->lightShadowCubeMapLocation[i] = this->lightShadowCubeMapLocation[i];
                baseMaterial->lightAngularShadowBiasLocation[i] = this->lightAngularShadowBiasLocation[i];
                baseMaterial->lightConstantShadowBiasLocation[i] = this->lightConstantShadowBiasLocation[i];
                baseMaterial->lightShadowMapSizeLocation[i] = this->lightShadowMapSizeLocation[i];
                baseMaterial->lightShadowSoftnessLocation[i] = this->lightShadowSoftnessLocation[i];
                baseMaterial->lightNearPlaneLocation[i] = this->lightNearPlaneLocation[i];
            }
            baseMaterial->lightCountLocation = this->lightCountLocation;
        } else {
            throw InvalidArgumentException("BaseLitMaterial::copyTo() -> 'target must be same material.");
        }
    }

    void BaseLitMaterial::bindShaderVarLocations() {
        BaseMaterial::bindShaderVarLocations();
        for (UInt32 i = 0; i < this->maxLightCount(); i++) {
            this->lightPositionLocation[i] = this->shader->getUniformLocation(StandardUniform::LightPosition, i);
            this->lightDirectionLocation[i] = this->shader->getUniformLocation(StandardUniform::LightDirection, i);
            this->lightRangeLocation[i] = this->shader->getUniformLocation(StandardUniform::LightRange, i);
            this->lightTypeLocation[i] = this->shader->getUniformLocation(StandardUniform::LightType, i);
            this->lightIntensityLocation[i] = this->shader->getUniformLocation(StandardUniform::LightIntensity, i);
            this->lightColorLocation[i] = this->shader->getUniformLocation(StandardUniform::LightColor, i);
            this->lightEnabledLocation[i] = this->shader->getUniformLocation(StandardUniform::LightEnabled, i);
            this->lightShadowsEnabledLocation[i] = this->shader->getUniformLocation(StandardUniform::LightShadowsEnabled, i);
            this->lightMatrixLocation[i] = this->shader->getUniformLocation(StandardUniform::LightMatrix, i);
            for (UInt32 c = 0; c < Constants::MaxDirectionalCascades; c++) {
                UInt32 cascadeIndex = Constants::MaxDirectionalCascades * i + c;
                this->lightViewProjectionLocations[cascadeIndex] = this->shader->getUniformLocation(StandardUniform::LightViewProjection, cascadeIndex);
                this->lightShadowMapLocations[cascadeIndex] = this->shader->getUniformLocation(StandardUniform::LightShadowMap, cascadeIndex);
                this->lightCascadeEndLocations[cascadeIndex] = this->shader->getUniformLocation(StandardUniform::LightCascadeEnd, cascadeIndex);
                this->lightShadowMapAspectLocations[cascadeIndex] = this->shader->getUniformLocation(StandardUniform::LightShadowMapAspect, cascadeIndex);
            }
            this->lightCascadeCountLocation[i] = this->shader->getUniformLocation(StandardUniform::LightCascadeCount, i);
            this->lightShadowCubeMapLocation[i] = this->shader->getUniformLocation(StandardUniform::LightShadowCubeMap, i);
            this->lightAngularShadowBiasLocation[i] = this->shader->getUniformLocation(StandardUniform::LightAngularShadowBias, i);
            this->lightConstantShadowBiasLocation[i] = this->shader->getUniformLocation(StandardUniform::LightConstantShadowBias, i);
            this->lightShadowMapSizeLocation[i] = this->shader->getUniformLocation(StandardUniform::LightShadowMapSize, i);
            this->lightShadowSoftnessLocation[i] = this->shader->getUniformLocation(StandardUniform::LightShadowSoftness, i);
            this->lightNearPlaneLocation[i] = this->shader->getUniformLocation(StandardUniform::LightNearPlane, i);
        }
        this->lightCountLocation = this->shader->getUniformLocation(StandardUniform::LightCount);
    }
}