#pragma once

#include "Texture.h"
#include "RawImage.h"

namespace Core {

  class CubeTexture : public Texture {
  public:
    std::weak_ptr<CubeTexture> build(RawImage *frontData, RawImage *backData,
                                     RawImage *topData, RawImage *bottomData,
                                     RawImage *leftData, RawImage *rightData);
  };

}
