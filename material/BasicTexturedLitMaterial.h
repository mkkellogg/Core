#pragma once

#include "../util/WeakPointer.h"
#include "Material.h"
#include "../common/Constants.h"
#include "../color/Color.h"

namespace Core {

    // forward declarations
    class Engine;
    class Texture;

    class BasicTexturedLitMaterial : public Material {
        friend class Engine;

    public:
        virtual Bool build() override;
        virtual Int32 getShaderLocation(StandardAttribute attribute, UInt32 offset = 0) override;
        virtual Int32 getShaderLocation(StandardUniform uniform, UInt32 offset = 0) override;
        virtual void sendCustomUniformsToShader() override;
        virtual WeakPointer<Material> clone() override;
        void setAlbedoMapEnabled(Bool enabled);
        void setAlbedoMap(WeakPointer<Texture> albedoMap);
        void setNormalMapEnabled(Bool enabled);
        void setNormalMap(WeakPointer<Texture> normalMap);
        virtual UInt32 textureCount() override;

    protected:
        BasicTexturedLitMaterial(WeakPointer<Graphics> graphics);
        void bindShaderVarLocations();

        Color albedo;
        PersistentWeakPointer<Texture> albedoMap;
        PersistentWeakPointer<Texture> normalMap;
        Bool albedoMapEnabled;
        Bool normalMapEnabled;

        Int32 positionLocation;
        Int32 normalLocation;
        Int32 faceNormalLocation;
        Int32 colorLocation;
        Int32 albedoMapEnabledLocation;
        Int32 normalMapEnabledLocation;
        Int32 albedoLocation;
        Int32 albedoMapLocation;
        Int32 albedoUVLocation;
        Int32 normalUVLocation;
        Int32 normalMapLocation;
        Int32 projectionMatrixLocation;
        Int32 viewMatrixLocation;
        Int32 modelMatrixLocation;
        Int32 modelInverseTransposeMatrixLocation;

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
