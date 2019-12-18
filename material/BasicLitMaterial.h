#pragma once

#include "../util/WeakPointer.h"
#include "BaseLitMaterial.h"
#include "../common/Constants.h"

namespace Core {

    // forward declarations
    class Engine;
    class Texture;

    class BasicLitMaterial : public BaseLitMaterial {
        friend class Engine;

    public:
        virtual Bool build() override;
        virtual WeakPointer<Material> clone() override;

    protected:
        BasicLitMaterial(WeakPointer<Graphics> graphics);
    };
}
