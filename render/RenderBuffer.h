#pragma once

namespace Core {

    enum class RenderBufferType {
        Color = 1,
        Depth = 2,
        Stencil = 4
    };

    enum class RenderBufferTypeIndex {
        Color = 0,
        Depth = 1,
        Stencil = 2
    };
    
}
