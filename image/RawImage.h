//
// Created by Mark Kellogg on 12/19/17.
//

#ifndef MPM_RAWIMAGE_H
#define MPM_RAWIMAGE_H

#include "../common/types.h"

namespace Core {
  class RawImage {
    Byte* imageBytes;
    UInt32 width;
    UInt32 height;
  public:
    RawImage(Byte* imageBytes, UInt32 width, UInt32 height);
    virtual ~RawImage();

    Byte* getImageData();
    UInt32 getWidth();
    UInt32 getHeight();
  };
}


#endif //MPM_RAWIMAGE_H
