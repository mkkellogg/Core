#pragma once

#include "../base/VectorStorage.h"
#include "../common/types.h"
#include "Vector2Components.h"
#include "../math/Math.h"

namespace Core {

#define VECTOR2_COMPONENT_COUNT 2

    template <typename T, bool customStorage = false, typename _chk = Core::enable_if_t<Core::is_numeric<T>::value>>
    class Vector2 : public VectorStorage<T, VECTOR2_COMPONENT_COUNT, customStorage>, public Vector2Components<T> {
    public:
        Vector2() : Vector2(0.0, 0.0) {}
        Vector2(const T& x, const T& y) : Vector2Components<T>(this->data, x, y) {}
        Vector2(const Vector2<T, true>& src) : Vector2(src.x, src.y) {}
        Vector2(const Vector2<T, false>& src) : Vector2(src.x, src.y) {}
        Vector2(T* storage) : Vector2(storage, 0.0, 0.0) {}
        Vector2(T* storage, const T& x, const T& y) : VectorStorage<T, VECTOR2_COMPONENT_COUNT, true>(storage), Vector2Components<T>(this->data, x, y) {}

        template <Bool otherCustomStorage>
        void copy(const Vector2<T, otherCustomStorage>& other) {
            this->x = other.x;
            this->y = other.y;
        }

        Vector2& operator=(const Vector2& other) {
            if (this == &other) return *this;
            this->copy(other);
            return *this;
        }

        template <Bool otherCustomStorage>
        Vector2& operator=(const Vector2<T, otherCustomStorage>& other) {
            this->copy(other);
            return *this;
        }

        void normalize() {
            Real magnitude = this->magnitude();
            if (magnitude != 0) {
                this->scale(1 / magnitude);
            }
        }

        void scale(Real magnitude) {
            this->x *= magnitude;
            this->y *= magnitude;
        }

        T magnitude() const {
            return Vector2<T, customStorage>::magnitude(this->x, this->y);
        }

        static T magnitude(const T& x, const T& y) {
            return Math::squareRoot(x * x + y * y);
        };

        Real squareMagnitude() const {
            return Vector2<T, customStorage>::squareMagnitude(this->x, this->y);
        }

        static T squareMagnitude(const T& x, const T& y) {
            return x * x + y * y;
        };

        Bool isZeroLength() const {
            Real epsilon = (Real)1e-06;
            Real sqlen = (this->x * this->x) + (this->y * this->y);
            return (sqlen < (epsilon * epsilon));
        }

        Real dot(const Vector2& other) {
            return Vector2::dot(*this, other);
        }

        static Real dot(const Vector2Components<Real>& a, const Vector2Components<Real>& b) {
            return a.x * b.x + a.y * b.y;
        }
    };

    typedef Vector2<Real, false> Vector2r;
    typedef Vector2<Real, true> Vector2rs;

    typedef Vector2<Int32, false> Vector2i;
    typedef Vector2<Int32, true> Vector2is;

    typedef Vector2<UInt32, false> Vector2u;
    typedef Vector2<UInt32, true> Vector2us;
}
