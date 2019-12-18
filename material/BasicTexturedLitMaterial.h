#pragma once

#include "../util/WeakPointer.h"
#include "BaseLitMaterial.h"
#include "../common/Constants.h"
#include "../color/Color.h"

namespace Core {

    // forward declarations
    class Engine;
    class Texture;

    class BasicTexturedLitMaterial : public BaseLitMaterial {
        friend class Engine;

    public:
        virtual ~BasicTexturedLitMaterial();
        virtual Bool build() override;
        virtual Int32 getShaderLocation(StandardAttribute attribute, UInt32 offset) override;
        virtual void sendCustomUniformsToShader() override;
        virtual void copyTo(WeakPointer<Material> target) override;
        virtual WeakPointer<Material> clone() override;
        virtual void bindShaderVarLocations() override;
        virtual UInt32 textureCount() override;

        void setAlbedoMapEnabled(Bool enabled);
        void setAlbedoMap(WeakPointer<Texture> albedoMap);
        void setNormalMapEnabled(Bool enabled);
        void setNormalMap(WeakPointer<Texture> normalMap);

    protected:
        BasicTexturedLitMaterial(WeakPointer<Graphics> graphics);

        Color albedo;
        PersistentWeakPointer<Texture> albedoMap;
        PersistentWeakPointer<Texture> normalMap;
        Bool albedoMapEnabled;
        Bool normalMapEnabled;

        Int32 albedoMapEnabledLocation;
        Int32 normalMapEnabledLocation;
        Int32 albedoLocation;
        Int32 albedoMapLocation;
        Int32 albedoUVLocation;
        Int32 normalUVLocation;
        Int32 normalMapLocation;
    };
}
