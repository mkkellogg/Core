#pragma once

#include "../base/VectorStorage.h"
#include "../common/types.h"
#include "Vector2Components.h"

namespace Core {

#define VECTOR2_COMPONENT_COUNT 2

    template <typename T, bool customStorage, typename Enable = void>
    class Vector2;

    template <typename T, bool customStorage>
    class Vector2<T, customStorage, Core::enable_if_t<Core::is_numeric<T>::value>> : public VectorStorage<T, VECTOR2_COMPONENT_COUNT, customStorage>,
                                                                                     public Vector2Components<T> {
    public:
        Vector2() : Vector2(0.0, 0.0) {}
        Vector2(const T& x, const T& y) : Vector2Components<T>(this->data, x, y) {}
        Vector2(T* storage) : Vector2(storage, 0.0, 0.0) {}
        Vector2(T* storage, const T& x, const T& y) : VectorStorage<T, VECTOR2_COMPONENT_COUNT, true>(storage), Vector2Components<T>(this->data, x, y) {}

        Vector2& operator=(const Vector2& other) {
            if (this == &other) return *this;
            this->copy(other);
            return *this;
        }
    };

    typedef Vector2<Real, false> Vector2r;
    typedef Vector2<Real, true> Vector2rs;

    typedef Vector2<Int32, false> Vector2i;
    typedef Vector2<Int32, true> Vector2is;

    typedef Vector2<UInt32, false> Vector2u;
    typedef Vector2<UInt32, true> Vector2us;
}
