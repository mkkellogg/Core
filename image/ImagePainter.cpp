#include "ImagePainter.h"
#include "../math/Math.h"

namespace Core {

    ImagePainter::ImagePainter(RawImage * targetImage): targetImage(targetImage) {

    }

    void ImagePainter::setDrawColor(IntColor4 color) {
        this->drawState.drawColor = color;
    }

    void ImagePainter::setFillColor(IntColor4 color) {
        this->drawState.fillColor = color;
    }

    void ImagePainter::drawLine(Int32 sx, Int32 sy, Int32 ex, Int32 ey) {
        this->clipLine(sx, sy, ex, ey);

        UInt32 xInc = ex - sx;
        UInt32 yInc = ey - sy;

        if (xInc == 0 && yInc == 0) return;

        if (xInc == 0) {
            drawVerticalLine(sx, sy, yInc);
        }
        else if (yInc == 0) {
            drawHorizontalLine(sx, sy, xInc);
        }

        UInt32 x = sx;
        UInt32 y = sy;

        Int32 dx = xInc > 0 ? 1 : -1;
        Int32 dy = yInc > 0 ? 1 : -1;
     
        xInc = Math::IAbs(xInc);
        yInc = Math::IAbs(yInc);

        UInt32 smallInc = xInc < yInc ? xInc : yInc;
        UInt32 bigInc = xInc + yInc - smallInc;
        Bool smallerFlag = smallInc == xInc ? false : true;
        UInt32 smallPixels = smallerFlag == 0 ? xInc : yInc;
        UInt32 smallCount = 0;
        UInt32 bigCount = 0;
        UInt32 lastDelta = 0;

        UInt32 width = this->targetImage->getWidth();
        UInt32 height = this->targetImage->getHeight();

        while (smallCount < smallPixels) {
            UInt32 tBigCount = bigCount;
            UInt32 p = 0;
            while (tBigCount - bigCount < bigInc - lastDelta) {
                if (x >=0 && x < width && y >= 0 && y < height) {
                    this->setPixel(x, y);
                }
                tBigCount += smallInc;
                p++;
                if (smallerFlag) x += dx;
                else y += dy;
            }
            lastDelta = (tBigCount - bigCount) - (p * smallInc);
            bigCount = tBigCount;

            smallCount++;
        }
    }

    void ImagePainter::setPixel(Int32 x, Int32 y) {
        Int32 oldY = y;
        Int32 oldX = x;
        x = this->clipX(x);
        y = this->clipY(y);
        if (y != oldY || x != oldX) return;
        Byte * location = this->targetImage->calcOffsetLocation(x, y);
        IntColor4& color = this->drawState.drawColor;
        location[0] = color.r;
        location[1] = color.g;
        location[2] = color.b;
        location[3] = color.a;
    }

    void ImagePainter::drawHorizontalLine(Int32 x, Int32 y, Int32 length) {
        Int32 ex = x + length;
        Int32 oldY = y;
        x = this->clipX(x);
        y = this->clipY(y);
        if (y != oldY) return;
        ex = this->clipX(ex);
        Int32 finalLength = ex - x;
        if(finalLength > 0) {
            Byte * location = this->targetImage->calcOffsetLocation(x, y);
            UInt32 byteCount = this->targetImage->calcRowSize(finalLength);
            if (byteCount % 4 != 0) {
                throw PaintException("ImagePainter::drawHorizontalLine() -> image byte count must be multiple of 4.");
            }
            IntColor4 drawColor = this->drawState.drawColor;
            for (UInt32 i = 0; i < byteCount; i += 4) {
                location[i] = drawColor.r;
                location[i + 1] = drawColor.g;
                location[i + 2] = drawColor.b;
                location[i + 3] = drawColor.a;
            }
        }
    }

    void ImagePainter::drawVerticalLine(Int32 x, Int32 y, Int32 length) {
        Int32 ey = y + length;
        Int32 oldX = x;
        x = this->clipX(x);
        y = this->clipY(y);
        if (x != oldX) return;
        ey = this->clipY(ey);

        Int32 finalLength = ey - y;
        if(finalLength > 0) {
            Byte * location = this->targetImage->calcOffsetLocation(x, y);
            UInt32 rowSize = this->targetImage->calcRowSize();
            if (rowSize % 4 != 0) {
                throw PaintException("ImagePainter::drawHorizontalLine() -> image byte count must be multiple of 4.");
            }
            IntColor4 drawColor = this->drawState.drawColor;
            for (UInt32 i = 0; i < finalLength; i ++) {
                location[0] = drawColor.r;
                location[1] = drawColor.g;
                location[2] = drawColor.b;
                location[3] = drawColor.a;
                location += rowSize;
            }
        }
    }

    void ImagePainter::drawRect(Int32 sx, Int32 sy, Int32 width, Int32 height) {

    }

    void ImagePainter::fillRect(Int32 sx, Int32 sy, Int32 width, Int32 height) {

    }

    void ImagePainter::clipLine(Int32& sx, Int32& sy, Int32& ex, Int32& ey) const {
        Int32 dx = ex - sx;
        Int32 dy = ey - sy;

        if (sx < 0) {

        }
    }

    Int32 ImagePainter::clipX(Int32 x) const {
       return clip(x, true);
    }

    Int32 ImagePainter::clipY(Int32 y) const {
        return clip(y, false);
    }

    inline Int32 ImagePainter::clip(Int32 v, Bool horizontal) const {
        Int32 max = horizontal ? this->targetImage->getWidth() : this->targetImage->getHeight();
        if (v > max) v = max;
        if (v < 0) v = 0;
        return v;
    }
}