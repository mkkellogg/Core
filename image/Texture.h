#pragma once

#include <memory>
#include "../common/gl.h"
#include "Texture.h"
#include "RawImage.h"

namespace Core {

  // forward declaration
  class Engine;

  class Texture {
    friend class Engine;

  public:
    ~Texture();

    GLuint getTextureID();
    static void createTexture(RawImage *imageData, std::weak_ptr<Texture> texture);

  protected:
    Texture();
    GLuint textureId;
  };

}
