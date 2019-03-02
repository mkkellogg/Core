#pragma once

#include "../util/WeakPointer.h"
#include "Material.h"
#include "../common/Constants.h"

namespace Core {

    // forward declarations
    class Engine;
    class Texture;

    class StandardPhysicalMaterial : public Material {
        friend class Engine;

    public:
        virtual Bool build() override;
        virtual Int32 getShaderLocation(StandardAttribute attribute, UInt32 offset = 0) override;
        virtual Int32 getShaderLocation(StandardUniform uniform, UInt32 offset = 0) override;
        virtual void sendCustomUniformsToShader() override;
        virtual WeakPointer<Material> clone() override;
        void setMetallic(Real metallic);
        void setRoughness(Real roughness);
        void setTexture(WeakPointer<Texture> texture);
        virtual UInt32 textureCount() override;

    protected:
        StandardPhysicalMaterial(WeakPointer<Graphics> graphics);
        void bindShaderVarLocations();

        Real metallic;
        Real roughness;
        WeakPointer<Texture> texture;

        Int32 positionLocation;
        Int32 normalLocation;
        Int32 faceNormalLocation;
        Int32 colorLocation;
        Int32 textureLocation;
        Int32 uvLocation;
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
        Int32 lightMatrixLocation;
        Int32 lightShadowCubeMapLocation;
        Int32 lightAngularShadowBiasLocation;
        Int32 lightConstantShadowBiasLocation;
        Int32 lightShadowMapSizeLocation;
        Int32 lightShadowSoftnessLocation;
        Int32 lightNearPlaneLocation;
        Int32 lightCountLocation;
        Int32 cameraPositionLocation;
        Int32 metallicLocation;
        Int32 roughnessLocation;
    };
}
