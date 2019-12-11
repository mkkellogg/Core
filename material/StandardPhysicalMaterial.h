#pragma once

#include "../util/WeakPointer.h"
#include "ShaderMaterial.h"
#include "../color/Color.h"
#include "../common/Constants.h"

namespace Core {

    // forward declarations
    class Engine;
    class Texture;

    class StandardPhysicalMaterial : public ShaderMaterial {
        friend class Engine;

    public:
        virtual ~StandardPhysicalMaterial();
        virtual Bool build() override;
        virtual Int32 getShaderLocation(StandardAttribute attribute, UInt32 offset = 0) override;
        virtual Int32 getShaderLocation(StandardUniform uniform, UInt32 offset = 0) override;
        virtual void sendCustomUniformsToShader() override;
        virtual WeakPointer<Material> clone() override;
        void setMetallic(Real metallic);
        void setRoughness(Real roughness);
        void setAmbientOcclusion(Real ambientOcclusion);
        void setAlbedo(Color albedo);
        void setAlbedoMap(WeakPointer<Texture> albedoMap);
        void setNormalMap(WeakPointer<Texture> normalMap);
        void setRoughnessMap(WeakPointer<Texture> roughnessMap);
        void setMetallicMap(WeakPointer<Texture> roughnessMap);
        void setAlbedoMapEnabled(Bool enabled);
        void setNormalMapEnabled(Bool enabled);
        void setRoughnessMapEnabled(Bool enabled);
        void setMetallicMapEnabled(Bool enabled);
        virtual UInt32 textureCount() override;
        virtual void copyTo(WeakPointer<Material> targetMaterial) override;
        virtual void bindShaderVarLocations() override;

    protected:
        const UInt32 ALBEDO_MAP_MASK = 0x1;
        const UInt32 NORMAL_MAP_MASK = 0x1 << 1;
        const UInt32 ROUGHNESS_MAP_MASK = 0x1 << 2;
        const UInt32 METALLIC_MAP_MASK = 0x1 << 3;

        StandardPhysicalMaterial(const std::string& vertexShader, const std::string& fragmentShader, WeakPointer<Graphics> graphics);
        StandardPhysicalMaterial(const std::string& buildInShaderName, WeakPointer<Graphics> graphics);
        StandardPhysicalMaterial(WeakPointer<Graphics> graphics);
        UInt32 getEnabledMapMask();

        Real metallic;
        Real roughness;
        Real ambientOcclusion;
        Color albedo;
        PersistentWeakPointer<Texture> albedoMap;
        PersistentWeakPointer<Texture> normalMap;
        PersistentWeakPointer<Texture> roughnessMap;
        PersistentWeakPointer<Texture> metallicMap;

        Bool albedoMapEnabled;
        Bool normalMapEnabled;
        Bool roughnessMapEnabled;
        Bool metallicMapEnabled;

        Int32 positionLocation;
        Int32 normalLocation;
        Int32 faceNormalLocation;
        Int32 colorLocation;
        Int32 tangentLocation;
        Int32 albedoUVLocation;
        Int32 normalUVLocation;
        Int32 albedoLocation;
        Int32 albedoMapLocation;
        Int32 normalMapLocation;
        Int32 roughnessMapLocation;
        Int32 metallicMapLocation;
        Int32 projectionMatrixLocation;
        Int32 viewMatrixLocation;
        Int32 modelMatrixLocation;
        Int32 modelInverseTransposeMatrixLocation;

        Int32 skinningEnabledLocation;
        Int32 bonesLocation[Constants::MaxBones];
        Int32 boneIndexLocation;
        Int32 boneWeightLocation;

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
        Int32 lightIrradianceMapLocation;
        Int32 lightSpecularIBLPreFilteredMapLocation;
        Int32 lightSpecularIBLBRDFMapLocation;
        Int32 lightCountLocation;
        Int32 cameraPositionLocation;
        Int32 metallicLocation;
        Int32 roughnessLocation;
        Int32 ambientOcclusionLocation;
        Int32 enabledMapLocation;
    
    private:
        void setInitialParams();
    };
}
