#include <memory>
#include <vector>

#include "CubeTexture.h"
#include "../common/Exception.h"
#include "RawImage.h"

namespace Core {

    CubeTexture::~CubeTexture() {

    }

    CubeTexture::CubeTexture(const TextureAttributes& attributes): Texture(attributes) {

    }
}