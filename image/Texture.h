#pragma once

#include <memory>
#include "../common/gl.h"
#include "Texture.h"
#include "RawImage.h"

namespace Core {

  class Texture {
  public:
    GLuint getTextureID();
    static std::shared_ptr<Texture> createTexture(RawImage *imageData);

  protected:
    GLuint textureId;
  };

}
