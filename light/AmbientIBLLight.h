#pragma once

#include "../util/PersistentWeakPointer.h"
#include "Light.h"
#include "../geometry/Vector3.h"

namespace Core {

    // forward declarations
    class Engine;
    class CubeTexture;
    class Texture2D;

    class AmbientIBLLight final : public Light {
        friend class Engine;

    public:
        ~AmbientIBLLight();
        void init() override;
        void setIrradianceMap(WeakPointer<CubeTexture> irradianceMap);
        WeakPointer<CubeTexture> getIrradianceMap();
        void setSpecularIBLPreFilteredMap(WeakPointer<CubeTexture> specularIBLPreFilteredMap);
        WeakPointer<CubeTexture> getSpecularIBLPreFilteredMap();
        void setSpecularIBLBRDFMap(WeakPointer<Texture2D> specularIBLBRDFMap);
        WeakPointer<Texture2D> getSpecularIBLBRDFMap();
              
    protected:
        AmbientIBLLight(WeakPointer<Object3D> owner);

        PersistentWeakPointer<CubeTexture> irradianceMap;
        PersistentWeakPointer<CubeTexture> specularIBLPreFilteredMap;
        PersistentWeakPointer<Texture2D> specularIBLBRDFMap;
    };
}
