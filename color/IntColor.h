#pragma once

#include "../common/types.h"
#include "Color.h"

namespace Core {

    class IntColor {
    public:
        IntColor(): r(255), g(255), b(255), a(255) {}
        IntColor(Byte gray): r(gray), g(gray), b(gray), a(gray) {}
        IntColor(Byte r, Byte g, Byte b, Byte a): r(r), g(g), b(b), a(a) {}

        IntColor(const Color4<>& src) {
            this->r = (Byte)src.r;
            this->g = (Byte)src.g;
            this->b = (Byte)src.b;
            this->a = (Byte)src.a;
        }

        Byte r;
        Byte g;
        Byte b;
        Byte a;
    };

}
