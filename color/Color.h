#pragma once

#include "../base/VectorStorage.h"
#include "../common/types.h"
#include "Color4Components.h"

namespace Core {

#define COLOR_COMPONENT_COUNT 4

    template <bool customStorage = false>
    class Color4 : public VectorStorage<Real, COLOR_COMPONENT_COUNT, customStorage>, public Color4Components {
    public:
        Color4() : Color4(0.0, 0.0, 0.0, 1.0) {}
        Color4(const Color4& color) : Color4(color.r, color.g, color.b, color.a) {}
        Color4(Real r, Real g, Real b, Real a) : Color4Components(this->data, r, g, b, a) {}
        Color4(Real* storage) : Color4(storage, 0.0, 0.0, 0.0, 0.0) {}
        Color4(Real* storage, Real r, Real g, Real b, Real a)
            : VectorStorage<Real, COLOR_COMPONENT_COUNT, true>(storage), Color4Components(this->data, r, g, b, a) {}

        Color4& operator =(const Color4& other) {
            if (this == &other) return *this;
            this->r = other.r;
            this->g = other.g;
            this->b = other.b;
            this->a = other.a;
            return *this;
        }

        template <Bool otherCustomStorage>
        Color4& operator =(const Color4<otherCustomStorage>& other) {
            this->r = other.r;
            this->g = other.g;
            this->b = other.b;
            this->a = other.a;
            return *this;
        }

        template <Bool otherCustomStorage>
        void copy(const Color4<otherCustomStorage>& other) {
            this->r = other.r;
            this->g = other.g;
            this->b = other.b;
            this->a = other.a;
        }
    };

    typedef Color4<false> Color;
    typedef Color4<true> ColorS;

}
