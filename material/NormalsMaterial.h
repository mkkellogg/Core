#pragma once

#include "../util/WeakPointer.h"
#include "BaseMaterial.h"

namespace Core {

    // forward declarations
    class Engine;

    class NormalsMaterial : public BaseMaterial {
        friend class Engine;

    public:
        virtual ~NormalsMaterial();
        virtual Bool build() override;
        virtual void sendCustomUniformsToShader() override;
        virtual WeakPointer<Material> clone() override;
        virtual void bindShaderVarLocations() override;
        void setConvertNormalsToViewSpace(Bool convertNormalsToViewSpace);

    private:
        NormalsMaterial(WeakPointer<Graphics> graphics);

        Int32 viewSpaceLocation;

        Bool convertNormalsToViewSpace;
    };
}
