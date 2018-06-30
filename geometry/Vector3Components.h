#pragma once

#include "../common/types.h"
#include "../math/Math.h"

namespace Core {

    template <typename T>
    class Vector3Components {
    public:
        T& x;
        T& y;
        T& z;

        Vector3Components(T* data, const T& x, const T& y, const T& z) : x(data[0]), y(data[1]), z(data[2]) {
            this->set(x, y, z);
        }

        virtual ~Vector3Components() = 0;

        Bool operator==(const Vector3Components<T>& other) {
            if (this == &other) return true;
            Real epsilon = .005f;
            return Math::abs(other.x - this->x) < epsilon && Math::abs(other.y - this->y) < epsilon && Math::abs(other.z - this->z) < epsilon;
        }

        virtual void set(const T& x, const T& y, const T& z) {
            this->x = x;
            this->y = y;
            this->z = z;
        }

        virtual void copy(const Vector3Components<T>& src) {
            this->set(src.x, src.y, src.z);
        }

        Vector3Components& operator=(const Vector3Components& other) {
            if (this == &other) return *this;
            this->copy(other);
        }
    };

    template <typename T>
    Vector3Components<T>::~Vector3Components() {
    }
}
