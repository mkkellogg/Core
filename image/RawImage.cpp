//
// Created by Mark Kellogg on 12/19/17.
//

#include "RawImage.h"

namespace Core {

  RawImage::RawImage(Byte* imageBytes, UInt32 width, UInt32 height):
      imageBytes(imageBytes), width(width), height(height) {
  }

  RawImage::~RawImage() {
    if (imageBytes) {
      delete imageBytes;
      imageBytes = nullptr;
    }
  }

  Byte* RawImage::getImageData() {
    return this->imageBytes;
  }

  UInt32 RawImage::getWidth() {
    return this->width;
  }

  UInt32 RawImage::getHeight() {
    return this->height;
  }

}