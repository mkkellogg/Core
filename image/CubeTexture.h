#pragma once

#include "Texture.h"
#include "RawImage.h"

namespace Core {

  class CubeTexture : public Texture {
  public:
    static std::shared_ptr<CubeTexture> createCubeTexture(RawImage *frontData, RawImage *backData,
                                      RawImage *topData, RawImage *bottomData,
                                      RawImage *leftData, RawImage *rightData);
  };

}
