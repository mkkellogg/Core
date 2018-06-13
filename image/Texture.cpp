#include "Texture.h"
#include "../common/Exception.h"
#include "../util/WeakPointer.h"

namespace Core {

    Texture::Texture(): textureId(-1) {

    }

    Texture::~Texture() {

    }

    Int32 Texture::getTextureID() {
        return this->textureId;
    }

};
