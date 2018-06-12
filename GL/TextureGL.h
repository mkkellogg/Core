#pragma once

#include <memory>
#include "../common/gl.h"
#include "../image/Texture.h"
#include "../image/RawImage.h"

namespace Core {

  // forward declaration
  class GraphicsGL;

  class TextureGL final: public Texture {
    friend class GraphicsGL;

  public:
    ~TextureGL();

    void build(RawImage *imageData);

  protected:
    TextureGL();

  };

}
