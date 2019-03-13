#pragma once

#include "../util/WeakPointer.h"
#include "SkyboxMaterial.h"

namespace Core {

    // forward declarations
    class Engine;

    class IrradianceRendererMaterial : public SkyboxMaterial {
        friend class Engine;
    public:

    protected:
        IrradianceRendererMaterial(WeakPointer<Graphics> graphics);
    };
}
