#pragma once

#include "../util/WeakPointer.h"
#include "../common/Constants.h"
#include "BaseMaterial.h"

namespace Core {

    // forward declarations
    class Engine;

    class DistanceOnlyMaterial : public BaseMaterial {
        friend class Engine;

    public:
        virtual ~DistanceOnlyMaterial();
        virtual Bool build() override;
        virtual WeakPointer<Material> clone() override;

    private:
        DistanceOnlyMaterial();
    };
}
