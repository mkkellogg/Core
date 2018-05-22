//
// Created by Mark Kellogg on 12/19/17.
//

#ifndef MPM_CUBETEXTURE_H
#define MPM_CUBETEXTURE_H

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


#endif //MPM_CUBETEXTURE_H
