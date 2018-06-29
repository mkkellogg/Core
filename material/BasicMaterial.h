#pragma once

#include "../util/WeakPointer.h"
#include "Material.h"

namespace Core {

    // forward declarations
    class Engine;

    class BasicMaterial : public Material {
        friend class Engine;

    public:
        virtual Bool build() override;
        virtual Int32 getShaderLocation(StandardAttribute attribute) override;
        virtual Int32 getShaderLocation(StandardUniform uniform) override;
        virtual void sendCustomUniformsToShader() override;
        virtual WeakPointer<Material> clone() override;

    private:
        BasicMaterial(WeakPointer<Graphics> graphics);
        void bindShaderVarLocations();

        Int32 positionLocation;
        Int32 colorLocation;
        Int32 uvLocation;
        Int32 projectionMatrixLocation;
        Int32 viewMatrixLocation;
        Int32 modelMatrixLocation;
    };
}
