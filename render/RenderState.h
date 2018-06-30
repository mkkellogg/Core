#pragma once

namespace Core {
    
    class RenderState {
    public:

        enum class BlendingMode {
            None,
            Additive,
            Custom,
        };

        enum class BlendingMethod {
            One,
            SrcAlpha,
            OneMinusSrcAlpha,
            DstAlpha,
            OneMinusDstAlpha,
            Zero,
            DstColor,
            OneMinusDstColor,
            SrcColor,
            OneMinusSrcColor
        };

        enum class FaceCulling {
            Back = 0,
            Front = 1,
            None = 2
        };

        enum class DepthBufferFunction {
            Always = 0,
            Less = 1,
            LessThanOrEqual = 2,
            Greater = 3,
            GreaterThanOrEqual = 4,
            Equal = 5
        };

    };
}
