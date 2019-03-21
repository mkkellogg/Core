#include "TextureAttr.h"

namespace Core {
    
    TextureAttributes::TextureAttributes() {
        UseAlpha = false;
        WrapMode = TextureWrap::Repeat;
        FilterMode = TextureFilter::Point;
        MipLevel = 0;
        IsDepthTexture = false;
        Format = TextureFormat::RGBA8;
    }

    TextureAttributes::~TextureAttributes() {

    }
    
}