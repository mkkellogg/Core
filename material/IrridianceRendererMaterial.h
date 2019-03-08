#pragma once

#include "../util/WeakPointer.h"
#include "SkyboxMaterial.h"

namespace Core {

    // forward declarations
    class Engine;

    class IrridianceRendererMaterial : public SkyboxMaterial {
        friend class Engine;
    public:

    protected:
        IrridianceRendererMaterial(WeakPointer<Graphics> graphics);
    };
}
