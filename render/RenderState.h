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

        enum class CullFace {
            Back = 0,
            Front = 1
        };

        enum class DepthFunction {
            Always = 0,
            Less = 1,
            LessThanOrEqual = 2,
            Greater = 3,
            GreaterThanOrEqual = 4,
            Equal = 5
        };

        enum class StencilFunction {
            Always = 0,
            Less = 1,
            LessThanOrEqual = 2,
            Greater = 3,
            GreaterThanOrEqual = 4,
            Equal = 5,
            NotEqual = 6,
            Never = 7
        };

        enum class StencilAction {
            Keep = 0,
            Zero = 1,
            Replace = 2,
            Increment = 3,
            IncrementWrap = 4,
            Decrement = 5,
            DecrementWrap = 6,
            Invert = 7
        };

    };
}
