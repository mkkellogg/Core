#pragma once

#include "../util/WeakPointer.h"
#include "Material.h"
#include "../color/Color.h"

namespace Core {

    // forward declarations
    class Engine;

    class BasicExtrusionMaterial : public Material {
        friend class Engine;

    public:
        virtual Bool build() override;
        virtual Int32 getShaderLocation(StandardAttribute attribute, UInt32 offset = 0) override;
        virtual Int32 getShaderLocation(StandardUniform uniform, UInt32 offset = 0) override;
        virtual void sendCustomUniformsToShader() override;
        virtual WeakPointer<Material> clone() override;

        void setColor(Color color);
        void setZOffset(Real offset);
        void setExtrusionFactor(Real extrusionFactor);

    private:
        BasicExtrusionMaterial(WeakPointer<Graphics> graphics);
        void bindShaderVarLocations();

        Color color;
        Real zOffset;
        Real extrusionFactor;

        Int32 positionLocation;
        Int32 normalLocation;
        Int32 averagedNormalLocation;
        Int32 colorLocation;
        Int32 projectionMatrixLocation;
        Int32 viewMatrixLocation;
        Int32 modelMatrixLocation;
        Int32 modelInverseTransposeMatrixLocation;
        Int32 zOffsetLocation;
        Int32 extrusionFactorLocation;
    };
}
