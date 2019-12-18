#pragma once

#include "../util/WeakPointer.h"
#include "../common/Constants.h"
#include "BaseMaterial.h"
#include "../color/Color.h"

namespace Core {

    // forward declarations
    class Engine;

    class BasicColoredMaterial: public BaseMaterial {
        friend class Engine;

    public:
        virtual ~BasicColoredMaterial();
        virtual Bool build() override;
        virtual void sendCustomUniformsToShader() override;
        virtual void copyTo(WeakPointer<Material> target) override;
        virtual WeakPointer<Material> clone() override;
        virtual void bindShaderVarLocations() override;

        void setObjectColor(Color color);
        void setZOffset(Real offset);

    private:
        BasicColoredMaterial(WeakPointer<Graphics> graphics);

        Color objectColor;
        Real zOffset;

        Int32 objectColorLocation;
        Int32 zOffsetLocation;
    };
}
