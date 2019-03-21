#pragma once

#include "../util/WeakPointer.h"
#include "SkyboxMaterial.h"

namespace Core {

    // forward declarations
    class Engine;

    class SpecularIBLRendererMaterial : public SkyboxMaterial {
        friend class Engine;
    public:
        virtual void sendCustomUniformsToShader() override;
        virtual WeakPointer<Material> clone() override;
        void setRoughness(Real roughness);

    protected:
        SpecularIBLRendererMaterial(WeakPointer<Graphics> graphics);
        virtual void copyTo(WeakPointer<Material> target) override;
        virtual void bindShaderVarLocations() override;

        Real roughness;
        Int32 roughnessLocation;
    };
}
