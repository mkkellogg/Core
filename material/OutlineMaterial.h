#pragma once

#include "../util/WeakPointer.h"
#include "../common/Constants.h"
#include "BaseMaterial.h"
#include "../color/Color.h"

namespace Core {

    // forward declarations
    class Engine;

    class OutlineMaterial: public BaseMaterial {
        friend class Engine;

    public:
        virtual Bool build() override;
        virtual void sendCustomUniformsToShader() override;
        virtual void copyTo(WeakPointer<Material> target) override;
        virtual WeakPointer<Material> clone() override;
        virtual void bindShaderVarLocations() override;

        void setOutlineColor(Color color);
        void setEdgeWidth(Real width);
        void setPctExtend(Real extend);
        void setAbsExtend(Real extend);

    private:
        OutlineMaterial(WeakPointer<Graphics> graphics);

        Color outlineColor;
        Real edgeWidth;
        Real pctExtend;
        Real absExtend;

        Int32 outlineColorLocation;
        Int32 edgeWidthLocation;
        Int32 pctExtendLocation;
        Int32 absExtendLocation;
    };
}
