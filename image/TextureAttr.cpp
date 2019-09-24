#include "TextureAttr.h"

namespace Core {
    
    TextureAttributes::TextureAttributes() {
        this->MipLevels = 0;
        this->IsDepthTexture = false;
        this->UseAlpha = false;
        this->FilterMode = TextureFilter::Point;
        this->WrapMode = TextureWrap::Repeat;
        this->Format = TextureFormat::RGBA8;
    }

    TextureAttributes::~TextureAttributes() {

    }
    
}