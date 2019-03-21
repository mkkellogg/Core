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

        virtual void set(const T& x, const T& y, const T& z) {
            this->x = x;
            this->y = y;
            this->z = z;
        }
    };

    template <typename T>
    Vector3Components<T>::~Vector3Components() {
    }
}
