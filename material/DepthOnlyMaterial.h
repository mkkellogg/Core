#pragma once

#include "../util/WeakPointer.h"
#include "../common/Constants.h"
#include "BaseMaterial.h"

namespace Core {

    // forward declarations
    class Engine;

    class DepthOnlyMaterial : public BaseMaterial {
        friend class Engine;

    public:
        virtual ~DepthOnlyMaterial();
        virtual Bool build() override;
        virtual WeakPointer<Material> clone();

    private:
        DepthOnlyMaterial(WeakPointer<Graphics> graphics);
    };
}
