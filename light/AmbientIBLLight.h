#pragma once

#include "../util/PersistentWeakPointer.h"
#include "Light.h"
#include "../geometry/Vector3.h"

namespace Core {

    // forward declarations
    class Engine;
    class CubeTexture;

    class AmbientIBLLight final : public Light {
        friend class Engine;

    public:
        ~AmbientIBLLight();
        void init() override;
        void setIBLTexture(WeakPointer<CubeTexture> texture);
        WeakPointer<CubeTexture> getIBLTexture();
              
    protected:
        AmbientIBLLight(WeakPointer<Object3D> owner);

        PersistentWeakPointer<CubeTexture> iblTexture;
    };
}
