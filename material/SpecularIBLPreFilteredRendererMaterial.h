#pragma once

#include "../util/WeakPointer.h"
#include "SkyboxMaterial.h"

namespace Core {

    // forward declarations
    class Engine;

    class SpecularIBLPreFilteredRendererMaterial : public SkyboxMaterial {
        friend class Engine;
    public:
        virtual void sendCustomUniformsToShader() override;
        virtual WeakPointer<Material> clone() override;
        void setRoughness(Real roughness);
        void setTextureResolution(UInt32 resolution);

    protected:
        SpecularIBLPreFilteredRendererMaterial(WeakPointer<Graphics> graphics);
        virtual void copyTo(WeakPointer<Material> target) override;
        virtual void bindShaderVarLocations() override;

        Real roughness;
        UInt32 textureResolution;
        Int32 roughnessLocation;
        Int32 textureResolutionLocation;
    };
}
