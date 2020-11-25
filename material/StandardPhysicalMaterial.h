#pragma once

#include "../util/WeakPointer.h"
#include "ShaderMaterial.h"
#include "BaseLitMaterial.h"
#include "../color/Color.h"
#include "../common/Constants.h"

namespace Core {

    // forward declarations
    class Engine;
    class Texture;

    class StandardPhysicalMaterial : public ShaderMaterial<BaseLitMaterial> {
        friend class Engine;
        friend class StandardPhysicalMaterialMultiLight;

    public:
        virtual ~StandardPhysicalMaterial();
        virtual Bool build() override;
        virtual Int32 getShaderLocation(StandardAttribute attribute, UInt32 offset = 0) override;
        virtual Int32 getShaderLocation(StandardUniform uniform, UInt32 offset = 0) override;
        virtual void sendCustomUniformsToShader() override;
        virtual WeakPointer<Material> clone() override;
        virtual void copyTo(WeakPointer<Material> targetMaterial) override;
        virtual void bindShaderVarLocations() override;
        virtual UInt32 textureCount() override;

        void setMetallic(Real metallic);
        void setRoughness(Real roughness);
        void setAmbientOcclusion(Real ambientOcclusion);
        void setAlbedo(Color albedo);
        void setOpacity(Real opacity);
        void setAlbedoMap(WeakPointer<Texture> albedoMap);
        void setNormalMap(WeakPointer<Texture> normalMap);
        void setRoughnessMap(WeakPointer<Texture> roughnessMap);
        void setMetallicMap(WeakPointer<Texture> metallicMap);
        void setOpacityMap(WeakPointer<Texture> opacityMap);
        void setAlbedoMapEnabled(Bool enabled);
        void setNormalMapEnabled(Bool enabled);
        void setRoughnessMapEnabled(Bool enabled);
        void setMetallicMapEnabled(Bool enabled);
        void setOpacityMapEnabled(Bool enabled);
        virtual Bool hasOpacityMap() const override;
        virtual WeakPointer<Texture> getOpacityMap() override;
        void setOpacityChannelRedEnabled(Bool enabled);
        void setOpacityChannelAlphaEnabled(Bool enabled);
        void setDiscardMask(Byte mask);

    protected:
        const UInt32 ALBEDO_MAP_MASK = 0x1;
        const UInt32 NORMAL_MAP_MASK = 0x1 << 1;
        const UInt32 ROUGHNESS_MAP_MASK = 0x1 << 2;
        const UInt32 METALLIC_MAP_MASK = 0x1 << 3;
        const UInt32 OPACITY_MAP_MASK = 0x1 << 4;

        const UInt32 OPACITY_CHANNEL_RED_MASK = 0x1;
        const UInt32 OPACITY_CHANNEL_ALPHA_MASK = 0x1 << 3;

        StandardPhysicalMaterial(const std::string& vertexShader, const std::string& fragmentShader);
        StandardPhysicalMaterial(const std::string& buildInShaderName);
        StandardPhysicalMaterial();
        UInt32 getEnabledMapMask();
        UInt32 getEnabledOpacityChannelMask();

        Real metallic;
        Real roughness;
        Real ambientOcclusion;
        Color albedo;
        Real opacity;
        PersistentWeakPointer<Texture> albedoMap;
        PersistentWeakPointer<Texture> normalMap;
        PersistentWeakPointer<Texture> roughnessMap;
        PersistentWeakPointer<Texture> metallicMap;
        PersistentWeakPointer<Texture> opacityMap;

        Bool albedoMapEnabled;
        Bool normalMapEnabled;
        Bool roughnessMapEnabled;
        Bool metallicMapEnabled;
        Bool opacityMapEnabled;

        Bool opacityChannelRed;
        Bool opacityChannelAlpha;

        Byte discardMask;

        Int32 albedoUVLocation;
        Int32 normalUVLocation;

        Int32 albedoLocation;
        Int32 albedoMapLocation;
        Int32 normalMapLocation;
        Int32 roughnessMapLocation;
        Int32 metallicMapLocation;
        Int32 opacityMapLocation;

        Int32 metallicLocation;
        Int32 roughnessLocation;
        Int32 ambientOcclusionLocation;
        Int32 enabledMapLocation;
        Int32 opacityLocation;

        Int32 lightIrradianceMapLocation[Constants::MaxShaderLights];
        Int32 lightSpecularIBLPreFilteredMapLocation[Constants::MaxShaderLights];
        Int32 lightSpecularIBLBRDFMapLocation[Constants::MaxShaderLights];

        Int32 depthOutputOverrideLocation;
        Int32 enabledOpacityChannelLocation;

        Int32 discardMaskLocation;
    
    private:
        void setInitialParams();
    };
}
