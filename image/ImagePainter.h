#pragma once

#include "../common/types.h"
#include "../color/IntColor.h"
#include "RawImage.h"
#include "../common/Exception.h"

namespace Core {

    class ImagePainter {
    public:

        class PaintException: Exception {
        public:
            PaintException(const std::string& msg): Exception(msg) {}
            PaintException(const char* msg): Exception(msg) {}
        };

        ImagePainter(RawImage * targetImage);
        void setDrawColor(IntColor4 color);
        void setFillColor(IntColor4 color);
        void setPixel(Int32 x, Int32 y);
        void drawLine(Int32 sx, Int32 sy, Int32 ex, Int32 ey);
        void drawHorizontalLine(Int32 sx, Int32 sy, Int32 length);
        void drawVerticalLine(Int32 sx, Int32 sy, Int32 length);
        void drawRect(Int32 sx, Int32 sy, Int32 width, Int32 height);
        void fillRect(Int32 sx, Int32 sy, Int32 width, Int32 height);

    private:
        inline void clipLine(Int32& sx, Int32& sy, Int32& ex, Int32& ey) const;
        inline Int32 clipX(Int32 x) const;
        inline Int32 clipY(Int32 y) const;
        inline Int32 clip(Int32 v, Bool horizontal = true) const;

        class DrawState {
        public:
            DrawState() {}

            IntColor4 drawColor;
            IntColor4 fillColor;
        };

        DrawState drawState;
        RawImage * targetImage;
    };

}
