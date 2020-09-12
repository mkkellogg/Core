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
        virtual ~Skybox();
        void build(WeakPointer<CubeTexture> skyboxTexture, Bool isPhysical = false, Real exposure = 1.0f);
        WeakPointer<Object3D> getSkyboxObject();
        WeakPointer<MeshContainer> getSkyboxMeshContainer();
    private:
        PersistentWeakPointer<Object3D> skyboxObj;
        PersistentWeakPointer<MeshContainer> skyboxMeshContainer;
    };

}
