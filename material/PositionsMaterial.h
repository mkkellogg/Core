#pragma once

#include "../util/WeakPointer.h"
#include "BaseMaterial.h"

namespace Core {

    // forward declarations
    class Engine;

    class PositionsMaterial : public BaseMaterial {
        friend class Engine;

    public:
        virtual ~PositionsMaterial();
        virtual Bool build() override;
        virtual void sendCustomUniformsToShader() override;
        virtual void copyTo(WeakPointer<Material> target) override;
        virtual WeakPointer<Material> clone() override;
        virtual void bindShaderVarLocations() override;
        void setConvertToViewSpace(Bool convertToViewSpace);

    private:
        PositionsMaterial(WeakPointer<Graphics> graphics);

        Int32 viewSpaceLocation;

        Bool convertToViewSpace;
    };
}
