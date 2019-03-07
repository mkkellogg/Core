#pragma once

#include "../util/WeakPointer.h"
#include "SkyboxMaterial.h"

namespace Core {

    // forward declarations
    class Engine;

    class PhysicalSkyboxMaterial : public SkyboxMaterial {
        friend class Engine;

    protected:
        PhysicalSkyboxMaterial(WeakPointer<Graphics> graphics);
    };
}
