#pragma once

#include "types.h"

namespace Core {

    class Constants {
    public:
        static const UInt32 MaxDirectionalCascades = 3;
        static const UInt32 MaxShaderLights = 4;
        #ifdef CORE_USE_PRIVATE_INCLUDES
        static constexpr UInt32 TempRenderTargetSize = 4096;
        #endif
    };
}