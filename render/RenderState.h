#pragma once

namespace Core {
    
    class RenderState {
    public:

        enum class BlendingMode {
            None = 0,
            Standard = 1,
            Additive = 2,
            Subtractive = 3,
            Multiply = 4,
            Custom = 5
        };

        enum class BlendingEquation {
            Add = 0,
            Subtract = 1,
            ReverseSubtract = 2,
            Min = 3,
            Max = 4
        };

        enum class BlendingFactor {
            Zero = 0,
            One = 1,
            SrcColor = 2,
            OneMinusSrcColor = 3,
            SrcAlpha = 4,
            OneMinusSrcAlpha = 5,
            DstAlpha = 6,
            OneMinusDstAlpha = 7,
            DstColor = 8,
            OneMinusDstColor = 9,
            SrcAlphaSaturate = 10
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
