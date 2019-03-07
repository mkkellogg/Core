#pragma once

#include "../Engine.h"
#include "../common/types.h"
#include "../util/WeakPointer.h"

class CubeTexture;

namespace Core {

    class Skybox {
    public:
        Skybox();
        void build(WeakPointer<CubeTexture> skyboxTexture, Bool isPhysical = false, Real exposure = 1.0f);
        WeakPointer<RenderableContainer<Mesh>> getSkyboxObject();
    private:
        WeakPointer<Core::RenderableContainer<Mesh>> skyboxObj;
    };

}
