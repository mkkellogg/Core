#pragma once

#include "../util/PersistentWeakPointer.h"
#include "Light.h"
#include "../geometry/Vector3.h"

namespace Core {

    // forward declarations
    class Engine;
    class RenderTargetCube;

    class AmbientLight final : public Light {
        friend class Engine;

    public:
        ~AmbientLight();

        void init() override;
              
    protected:
        AmbientLight(WeakPointer<Object3D> owner);

    };
}
