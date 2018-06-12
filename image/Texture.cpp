#include "Texture.h"
#include "../common/Exception.h"
#include "../util/ValidWeakPointer.h"

namespace Core {

    Texture::Texture(): textureId(-1) {

    }

    Texture::~Texture() {

    }

    Int32 Texture::getTextureID() {
        return this->textureId;
    }

};
