//
// Created by Mark Kellogg on 12/19/17.
//

#ifndef MPM_TEXTURE_H
#define MPM_TEXTURE_H

#include <memory>
#include "../common/gl.h"
#include "Texture.h"
#include "RawImage.h"

namespace Core {

  class Texture {

  protected:
    GLuint textureId;

  public:
    GLuint getTextureID();

    static std::shared_ptr<Texture> createTexture(RawImage *imageData);
  };

}


#endif //MPM_TEXTURE_H
