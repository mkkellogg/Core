#pragma once

#include "../util/WeakPointer.h"
#include "StandardPhysicalMaterial.h"
#include "BaseLitMaterial.h"
#include "../color/Color.h"
#include "../common/Constants.h"

namespace Core {

    // forward declarations
    class Engine;
    class Graphics;

    class StandardPhysicalMaterialMultiLight : public StandardPhysicalMaterial {
        friend class Engine;
    public:

        virtual UInt32 maxLightCount() const override;
        void copyAttributesFromStandardPhysicalMaterial(WeakPointer<StandardPhysicalMaterial> source);

    protected:
        StandardPhysicalMaterialMultiLight(WeakPointer<Graphics> graphics);
    };
}
