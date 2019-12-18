#pragma once

#include "../util/WeakPointer.h"
#include "BaseMaterial.h"

namespace Core {

    // forward declarations
    class Engine;

    class BasicMaterial : public BaseMaterial {
        friend class Engine;

    public:
        virtual Bool build() override;
        virtual WeakPointer<Material> clone() override;

    private:
        BasicMaterial(WeakPointer<Graphics> graphics);
    };
}
