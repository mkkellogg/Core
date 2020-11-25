#pragma once

#include "../util/WeakPointer.h"
#include "BaseMaterial.h"
#include "../color/Color.h"

namespace Core {

    // forward declarations
    class Engine;

    class BasicExtrusionMaterial : public BaseMaterial {
        friend class Engine;

    public:
        virtual Bool build() override;
        virtual void sendCustomUniformsToShader() override;
        virtual void copyTo(WeakPointer<Material> target) override;
        virtual WeakPointer<Material> clone() override;
        virtual void bindShaderVarLocations() override;

        void setObjectColor(Color color);
        void setZOffset(Real offset);
        void setExtrusionFactor(Real extrusionFactor);

    private:
        BasicExtrusionMaterial();

        Color objectColor;
        Real zOffset;
        Real extrusionFactor;

        Int32 objectColorLocation;
        Int32 averagedNormalLocation;
        Int32 zOffsetLocation;
        Int32 extrusionFactorLocation;
    };
}
