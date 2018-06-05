#pragma once

#include "../common/types.h"

namespace Core {
  class RawImage {
  public:
    RawImage(Byte* imageBytes, UInt32 width, UInt32 height);
    virtual ~RawImage();

    Byte* getImageData();
    UInt32 getWidth();
    UInt32 getHeight();
    
  private:
    Byte* imageBytes;
    UInt32 width;
    UInt32 height;
  };
}
