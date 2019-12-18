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
        virtual Int32 getShaderLocation(StandardUniform uniform, UInt32 offset = 0) override;
        virtual void bindShaderVarLocations();
        virtual void copyTo(WeakPointer<Material> target) override;

    protected:
        BaseLitMaterial(WeakPointer<Graphics> graphics);

        Int32 lightPositionLocation;
        Int32 lightDirectionLocation;
        Int32 lightViewProjectionLocations[Constants::MaxDirectionalCascades];
        Int32 lightShadowMapLocations[Constants::MaxDirectionalCascades];
        Int32 lightCascadeEndLocations[Constants::MaxDirectionalCascades];
        Int32 lightShadowMapAspectLocations[Constants::MaxDirectionalCascades];
        Int32 lightCascadeCountLocation;
        Int32 lightRangeLocation;
        Int32 lightTypeLocation;
        Int32 lightIntensityLocation;
        Int32 lightColorLocation;
        Int32 lightEnabledLocation;
        Int32 lightShadowsEnabledLocation;
        Int32 lightMatrixLocation;
        Int32 lightShadowCubeMapLocation;
        Int32 lightAngularShadowBiasLocation;
        Int32 lightConstantShadowBiasLocation;
        Int32 lightShadowMapSizeLocation;
        Int32 lightShadowSoftnessLocation;
        Int32 lightNearPlaneLocation;
        Int32 lightCountLocation;
    };
}
