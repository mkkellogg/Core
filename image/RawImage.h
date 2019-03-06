#pragma once

#include <memory.h>

#include "../common/types.h"
#include "../common/debug.h"
#include "../common/Exception.h"
#include "../color/IntColor.h"

namespace Core {

    template <typename T = Byte> class RawImage {
        friend class ImageLoader;

    public:
        RawImage(UInt32 width, UInt32 height) {
            this->width = width;
            this->height = height;
            imageData = nullptr;
        }

        ~RawImage() {
            destroy();
        }

        UInt32 imageSizeElements() {
            return height * this->calcRowSizeElements(this->width);
        }

        UInt32 imageSizeBytes() {
            return height * this->calcRowSizeBytes(this->width);
        }

        Bool init() {
            this->imageData = (T *)::operator new (imageSizeBytes(), std::nothrow);
            if (this->imageData == nullptr) {
                throw AllocationException("RawImage::init() -> Unable to allocate memory for raw image");
            }
            return true;
        }

        void setDataTo(T * data) {
            if (data == nullptr) {
                throw Exception("RawImage::setDataTo() -> [data] is null");
            }
            if (this->imageData != nullptr) {
                memcpy(this->imageData, data, imageSizeBytes());
            }
        }

        void setElement(UInt32 index, T element) {
            if (index < imageSizeElements()) {
                this->imageData[index] = element;
            }
            else {
                throw OutOfRangeException("RawImage::setElement() -> [index] is out of range");
            }
        }

        T * getImageData() {
            return imageData;
        }

        UInt32 getWidth() const {
            return width;
        }

        UInt32 getHeight() const {
            return height;
        }

        T* calcOffsetLocationElements(UInt32 x, UInt32 y) const {
            UInt32 rowSize = this->calcRowSizeElements();
            UInt32 columnOffset = this->calcRowSizeElements(x);
            return this->imageData + (rowSize * y + columnOffset);
        }

        Byte* calcOffsetLocationBytes(UInt32 x, UInt32 y) const {
            UInt32 rowSize = this->calcRowSizeBytes();
            UInt32 columnOffset = this->calcRowSizeBytes(x);
            return ((Byte*)this->imageData) + (rowSize * y + columnOffset);
        }

        UInt32 calcRowSizeElements() const {
            return this->calcRowSizeElements(this->width);
        }

        UInt32 calcRowSizeElements(UInt32 pixels) const {
            return pixels * 4;
        }

         UInt32 calcRowSizeBytes() const {
            return this->calcRowSizeBytes(this->width);
        }

        UInt32 calcRowSizeBytes(UInt32 pixels) const {
            return calcRowSizeElements(pixels) * sizeof(T);
        }
    
    private:
        void destroy() {
            if (this->imageData) {
                ::operator delete(this->imageData);
                this->imageData = nullptr;
            }
        }

        UInt32 width;
        UInt32 height;
        T * imageData;
    };

    using StandardImage = RawImage<Byte>;
}
