#include "Texture.h"
#include "../common/Exception.h"
#include "../util/WeakPointer.h"

namespace Core {

    Texture::Texture(const TextureAttributes& attributes): textureId(-1), attributes(attributes) {

    }

    Texture::~Texture() {

    }

    Int32 Texture::getTextureID() const {
        return this->textureId;
    }

    Bool Texture::isBuilt() const {
        return this->textureId > 0;
    }

};
