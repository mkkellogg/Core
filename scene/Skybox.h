#pragma once

#include "../Engine.h"
#include "../common/types.h"
#include "../util/WeakPointer.h"

class CubeTexture;

namespace Core {

    class Skybox {
    public:
        Skybox();
        void build(WeakPointer<CubeTexture> skyboxTexture);
    };

}
