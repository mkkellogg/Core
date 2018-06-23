#pragma once

#include "../common/types.h"
#include "../common/debug.h"
#include "../common/Exception.h"

namespace Core {

    class RawImage {
        friend class ImageLoader;

    public:
        RawImage(UInt32 width, UInt32 height);
        ~RawImage();

        Bool init();
        void setDataTo(Byte * data);
        void setByte(UInt32 index, Byte byte);

        Byte * getImageData();
        UInt32 getWidth() const;
        UInt32 getHeight() const;

    protected:
        UInt32 imageSizeBytes();
        void destroy();

        UInt32 width;
        UInt32 height;
        Byte * imageBytes;
    };

}
