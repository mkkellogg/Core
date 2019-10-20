#pragma once

#include "../Engine.h"
#include "../render/MeshContainer.h"
#include "../common/types.h"
#include "../util/WeakPointer.h"

class CubeTexture;

namespace Core {

    class Skybox {
    public:
        Skybox();
        void build(WeakPointer<CubeTexture> skyboxTexture, Bool isPhysical = false, Real exposure = 1.0f);
        WeakPointer<MeshContainer> getSkyboxObject();
    private:
        PersistentWeakPointer<MeshContainer> skyboxObj;
    };

}
