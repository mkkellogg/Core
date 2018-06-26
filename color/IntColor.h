#pragma once

#include "../common/types.h"
#include "Color.h"

namespace Core {

    class IntColor4 {
    public:
        IntColor4(): r(255), g(255), b(255), a(255) {}
        IntColor4(Byte gray): r(gray), g(gray), b(gray), a(gray) {}
        IntColor4(Byte r, Byte g, Byte b, Byte a): r(r), g(g), b(b), a(a) {}

        IntColor4(const Color4<>& src) {
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
