#pragma once

#include "../util/PersistentWeakPointer.h"
#include "Light.h"
#include "../geometry/Vector3.h"

namespace Core {

    // forward declarations
    class Engine;
    class CubeTexture;
    class Texture2D;
    class ReflectionProbe;

    class AmbientIBLLight final : public Light {
        friend class Engine;

    public:
        ~AmbientIBLLight() override;
        void init() override;
        void setIrradianceMap(WeakPointer<CubeTexture> irradianceMap);
        WeakPointer<CubeTexture> getIrradianceMap();
        void setSpecularIBLPreFilteredMap(WeakPointer<CubeTexture> specularIBLPreFilteredMap);
        WeakPointer<CubeTexture> getSpecularIBLPreFilteredMap();
        void setSpecularIBLBRDFMap(WeakPointer<Texture2D> specularIBLBRDFMap);
        WeakPointer<Texture2D> getSpecularIBLBRDFMap();
        void setReflectionProbe(WeakPointer<ReflectionProbe> reflectionProbe);
        void updateMapsFromReflectionProbe();
              
    protected:
        AmbientIBLLight(WeakPointer<Object3D> owner);

        PersistentWeakPointer<CubeTexture> irradianceMap;
        PersistentWeakPointer<CubeTexture> specularIBLPreFilteredMap;
        PersistentWeakPointer<Texture2D> specularIBLBRDFMap;
        PersistentWeakPointer<ReflectionProbe> reflectionProbe;
    };
}
