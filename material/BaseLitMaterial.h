#pragma once

#include "../util/WeakPointer.h"
#include "../common/Constants.h"
#include "BaseMaterial.h"
#include "../color/Color.h"

namespace Core {

    // forward declarations
    class Engine;

    class BaseLitMaterial: public BaseMaterial {
        friend class Engine;

    public:
        virtual ~BaseLitMaterial();
        virtual Int32 getShaderLocation(StandardUniform uniform, UInt32 offset = 0) override;
        virtual void bindShaderVarLocations();
        virtual void copyTo(WeakPointer<Material> target) override;

    protected:
        BaseLitMaterial();

        Int32 lightPositionLocation[Constants::MaxShaderLights];
        Int32 lightDirectionLocation[Constants::MaxShaderLights];
        Int32 lightViewProjectionLocations[Constants::MaxDirectionalCascades * Constants::MaxShaderLights];
        Int32 lightShadowMapLocations[Constants::MaxDirectionalCascades * Constants::MaxShaderLights];
        Int32 lightCascadeEndLocations[Constants::MaxDirectionalCascades * Constants::MaxShaderLights];
        Int32 lightShadowMapAspectLocations[Constants::MaxDirectionalCascades * Constants::MaxShaderLights];
        Int32 lightCascadeCountLocation[Constants::MaxShaderLights];
        Int32 lightRangeLocation[Constants::MaxShaderLights];
        Int32 lightTypeLocation[Constants::MaxShaderLights];
        Int32 lightIntensityLocation[Constants::MaxShaderLights];
        Int32 lightColorLocation[Constants::MaxShaderLights];
        Int32 lightEnabledLocation[Constants::MaxShaderLights];
        Int32 lightShadowsEnabledLocation[Constants::MaxShaderLights];
        Int32 lightMatrixLocation[Constants::MaxShaderLights];
        Int32 lightShadowCubeMapLocation[Constants::MaxShaderLights];
        Int32 lightAngularShadowBiasLocation[Constants::MaxShaderLights];
        Int32 lightConstantShadowBiasLocation[Constants::MaxShaderLights];
        Int32 lightShadowMapSizeLocation[Constants::MaxShaderLights];
        Int32 lightShadowSoftnessLocation[Constants::MaxShaderLights];
        Int32 lightNearPlaneLocation[Constants::MaxShaderLights];
        Int32 lightCountLocation;
    };
}
