#pragma once

#include "../common/types.h"
#include "../util/WeakPointer.h"

namespace Core {

    // forward declarations
    class CubeTexture;

    class TextureUtils {
    public:
        static WeakPointer<CubeTexture> loadFromEquirectangularImage(const std::string& filePath, Bool isHDR = false);

    };
}
