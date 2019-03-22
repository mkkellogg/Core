#pragma once

#include "../common/types.h"
#include "../color/Color.h"

namespace Core {

    enum class CubeTextureSide {
        Front = 0,
        Back = 1,
        Top = 2,
        Bottom = 3,
        Left = 4,
        Right = 5
    };

    enum class TextureWrap {
        Repeat,
        Clamp,
        Mirror,
        Border
    };

    enum class TextureFilter {
        Point,
        Linear,
        BiLinear,
        TriLinear,
        Anisotropic
    };

    enum class TextureFormat {
        RGBA8 = 1,
        RGBA16F = 2,
        RGBA32F = 3,
        R32F = 4,
        RG16F = 5,
        DEPTH = 6,
        DEPTH16 = 7,
        DEPTH24 = 8,
        DEPTH32 = 9,
    };

    class TextureAttributes {
    public:

        UInt32 MipLevels;
        Bool IsDepthTexture;
        Bool UseAlpha;
        TextureFilter FilterMode;
        TextureWrap WrapMode;
        TextureFormat Format;
        Color BorderWrapColor;

        TextureAttributes();
        ~TextureAttributes();
    };
    
}
