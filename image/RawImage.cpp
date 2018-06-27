#include <memory.h>

#include "RawImage.h"

namespace Core {

    RawImage::RawImage(UInt32 width, UInt32 height) {
        this->width = width;
        this->height = height;
        imageBytes = nullptr;
    }

    RawImage::~RawImage() {
        destroy();
    }

    void RawImage::destroy() {
        if (imageBytes) {
          ::operator delete(imageBytes);
          imageBytes = nullptr;
        }
    }

    UInt32 RawImage::imageSizeBytes() {
        return height * this->calcRowSize(this->width);
    }

    Bool RawImage::init() {
        imageBytes = (Byte *)::operator new (imageSizeBytes() * sizeof(Byte), std::nothrow);
        if (imageBytes == nullptr) {
          throw AllocationException("RawImage::init() -> Unable to allocate memory for raw image");
        }
        return true;
    }

    void RawImage::setDataTo(Byte * data) {
        if (data == nullptr) {
          throw Exception("RawImage::setDataTo() -> [data] is null");
        }
        if (imageBytes != nullptr) {
            memcpy(imageBytes, data, imageSizeBytes());
        }
    }

    void RawImage::setByte(UInt32 index, Byte byte) {
        if (index < imageSizeBytes()) {
          imageBytes[index] = byte;
        }
        else {
          throw OutOfRangeException("RawImage::setByte() -> [index] is out of range");
        }
    }

    Byte * RawImage::getImageData() {
        return imageBytes;
    }

    UInt32 RawImage::getWidth() const {
        return width;
    }

    UInt32 RawImage::getHeight() const {
        return height;
    }

    Byte* RawImage::calcOffsetLocation(UInt32 x, UInt32 y) const {
        UInt32 rowSize = this->calcRowSize();
        UInt32 columnOffset = this->calcRowSize(x);
        return this->imageBytes + (rowSize * y + columnOffset);
    }

    UInt32 RawImage::calcRowSize() const {
        return this->calcRowSize(this->width);
    }

    UInt32 RawImage::calcRowSize(UInt32 pixels) const {
        return pixels * 4;
    }

}