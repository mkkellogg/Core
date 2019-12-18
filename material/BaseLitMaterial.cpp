#include "BaseLitMaterial.h"
#include "StandardUniforms.h"
#include "../Engine.h"
#include "../Graphics.h"
#include "../material/ShaderManager.h"

namespace Core {

    BaseLitMaterial::BaseLitMaterial(WeakPointer<Graphics> graphics) : BaseMaterial(graphics) {
        this->lightPositionLocation = -1;
        this->lightDirectionLocation = -1;
        for (UInt32 i = 0; i < Constants::MaxDirectionalCascades; i++) {
            this->lightViewProjectionLocations[i] = -1;
            this->lightShadowMapLocations[i] = -1;
            this->lightCascadeEndLocations[i] = -1;
            this->lightShadowMapAspectLocations[i] = -1;
        }
        this->lightCascadeCountLocation = -1;
        this->lightRangeLocation = -1;
        this->lightTypeLocation = -1;
        this->lightIntensityLocation = -1;
        this->lightColorLocation = -1;
        this->lightEnabledLocation = -1;
        this->lightShadowsEnabledLocation = -1;
        this->lightMatrixLocation = -1;
        this->lightShadowCubeMapLocation = -1;
        this->lightAngularShadowBiasLocation = -1;
        this->lightConstantShadowBiasLocation = -1;
        this->lightShadowMapSizeLocation = -1;
        this->lightShadowSoftnessLocation = -1;
        this->lightNearPlaneLocation = -1;
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
             offset >= Constants::MaxDirectionalCascades) {
            throw InvalidArgumentException("BasicLitMaterial::getShaderLocation() -> invalid offset.");
        }

        switch (uniform) {
            case StandardUniform::LightPosition:
                return this->lightPositionLocation;
            case StandardUniform::LightDirection:
                return this->lightDirectionLocation;
            case StandardUniform::LightRange:
                return this->lightRangeLocation;
            case StandardUniform::LightType:
                return this->lightTypeLocation;
            case StandardUniform::LightIntensity:
                return this->lightIntensityLocation;
            case StandardUniform::LightColor:
                return this->lightColorLocation;
            case StandardUniform::LightEnabled:
                return this->lightEnabledLocation;
            case StandardUniform::LightShadowsEnabled:
                return this->lightShadowsEnabledLocation;
            case StandardUniform::LightMatrix:
                return this->lightMatrixLocation;
            case StandardUniform::LightViewProjection:
                return this->lightViewProjectionLocations[offset];
            case StandardUniform::LightShadowMap:
                return this->lightShadowMapLocations[offset];
            case StandardUniform::LightCascadeEnd:
                return this->lightCascadeEndLocations[offset];
            case StandardUniform::LightShadowMapAspect:
                return this->lightShadowMapAspectLocations[offset];
            case StandardUniform::LightCascadeCount:
                return this->lightCascadeCountLocation;
            case StandardUniform::LightShadowCubeMap:
                return this->lightShadowCubeMapLocation;
            case StandardUniform::LightAngularShadowBias:
                return this->lightAngularShadowBiasLocation;
            case StandardUniform::LightConstantShadowBias:
                return this->lightConstantShadowBiasLocation;
            case StandardUniform::LightShadowMapSize:
                return this->lightShadowMapSizeLocation;
            case StandardUniform::LightShadowSoftness:
                return this->lightShadowSoftnessLocation;
            case StandardUniform::LightNearPlane:
                return this->lightNearPlaneLocation;
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
            baseMaterial->lightPositionLocation = this->lightPositionLocation;
            baseMaterial->lightDirectionLocation = this->lightDirectionLocation;
            baseMaterial->lightRangeLocation = this->lightRangeLocation;
            baseMaterial->lightTypeLocation = this->lightTypeLocation;
            baseMaterial->lightIntensityLocation = this->lightIntensityLocation;
            baseMaterial->lightColorLocation = this->lightColorLocation;
            baseMaterial->lightEnabledLocation = this->lightEnabledLocation;
            baseMaterial->lightShadowsEnabledLocation = this->lightShadowsEnabledLocation;
            baseMaterial->lightMatrixLocation = this->lightMatrixLocation;
            for (UInt32 i =0; i < Constants::MaxDirectionalCascades; i++) {
                baseMaterial->lightViewProjectionLocations[i] = this->lightViewProjectionLocations[i];
                baseMaterial->lightShadowMapLocations[i] = this->lightShadowMapLocations[i];
                baseMaterial->lightCascadeEndLocations[i] = this->lightCascadeEndLocations[i];
                baseMaterial->lightShadowMapAspectLocations[i] = this->lightShadowMapAspectLocations[i];
            }
            baseMaterial->lightCascadeCountLocation = this->lightCascadeCountLocation;
            baseMaterial->lightShadowCubeMapLocation = this->lightShadowCubeMapLocation;
            baseMaterial->lightAngularShadowBiasLocation = this->lightAngularShadowBiasLocation;
            baseMaterial->lightConstantShadowBiasLocation = this->lightConstantShadowBiasLocation;
            baseMaterial->lightShadowMapSizeLocation = this->lightShadowMapSizeLocation;
            baseMaterial->lightShadowSoftnessLocation = this->lightShadowSoftnessLocation;
            baseMaterial->lightNearPlaneLocation = this->lightNearPlaneLocation;
            baseMaterial->lightCountLocation = this->lightCountLocation;
        } else {
            throw InvalidArgumentException("BaseLitMaterial::copyTo() -> 'target must be same material.");
        }
    }

    void BaseLitMaterial::bindShaderVarLocations() {
        BaseMaterial::bindShaderVarLocations();
        this->lightPositionLocation = this->shader->getUniformLocation(StandardUniform::LightPosition);
        this->lightDirectionLocation = this->shader->getUniformLocation(StandardUniform::LightDirection);
        this->lightRangeLocation = this->shader->getUniformLocation(StandardUniform::LightRange);
        this->lightTypeLocation = this->shader->getUniformLocation(StandardUniform::LightType);
        this->lightIntensityLocation = this->shader->getUniformLocation(StandardUniform::LightIntensity);
        this->lightColorLocation = this->shader->getUniformLocation(StandardUniform::LightColor);
        this->lightEnabledLocation = this->shader->getUniformLocation(StandardUniform::LightEnabled);
        this->lightShadowsEnabledLocation = this->shader->getUniformLocation(StandardUniform::LightShadowsEnabled);
        this->lightMatrixLocation = this->shader->getUniformLocation(StandardUniform::LightMatrix);
        for (UInt32 i =0; i < Constants::MaxDirectionalCascades; i++) {
            this->lightViewProjectionLocations[i] = this->shader->getUniformLocation(StandardUniform::LightViewProjection, i);
            this->lightShadowMapLocations[i] = this->shader->getUniformLocation(StandardUniform::LightShadowMap, i);
            this->lightCascadeEndLocations[i] = this->shader->getUniformLocation(StandardUniform::LightCascadeEnd, i);
            this->lightShadowMapAspectLocations[i] = this->shader->getUniformLocation(StandardUniform::LightShadowMapAspect, i);
        }
        this->lightCascadeCountLocation = this->shader->getUniformLocation(StandardUniform::LightCascadeCount);
        this->lightShadowCubeMapLocation = this->shader->getUniformLocation(StandardUniform::LightShadowCubeMap);
        this->lightAngularShadowBiasLocation = this->shader->getUniformLocation(StandardUniform::LightAngularShadowBias);
        this->lightConstantShadowBiasLocation = this->shader->getUniformLocation(StandardUniform::LightConstantShadowBias);
        this->lightShadowMapSizeLocation = this->shader->getUniformLocation(StandardUniform::LightShadowMapSize);
        this->lightShadowSoftnessLocation = this->shader->getUniformLocation(StandardUniform::LightShadowSoftness);
        this->lightNearPlaneLocation = this->shader->getUniformLocation(StandardUniform::LightNearPlane);
        this->lightCountLocation = this->shader->getUniformLocation(StandardUniform::LightCount);
    }
}