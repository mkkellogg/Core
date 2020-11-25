#pragma once

#include "../util/WeakPointer.h"
#include "../common/Constants.h"
#include "BaseMaterial.h"
#include "../color/Color.h"

namespace Core {

    // forward declarations
    class Engine;
    class Texture;

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

        void setHasOpacityMap(Bool hasOpacityMap);
        void setOpacityMap(WeakPointer<Texture> opacityMap);

    private:
        OutlineMaterial();

        Color outlineColor;
        Real edgeWidth;
        Real pctExtend;
        Real absExtend;

        Bool hasOpacityMap;
        PersistentWeakPointer<Texture> opacityMap;

        Int32 outlineColorLocation;
        Int32 edgeWidthLocation;
        Int32 pctExtendLocation;
        Int32 absExtendLocation;
        Int32 opacityMapEnabledLocation;
        Int32 opacityMapLocation;

    };
}
