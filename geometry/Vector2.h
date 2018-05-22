#pragma once

#include "../common/types.h"
#include "../base/VectorStorage.h"
#include "Vector2Components.h"

namespace Core {

  #define VECTOR2_COMPONENT_COUNT 2

  template <typename T, bool customStorage,
      typename = typename std::enable_if<Core::is_numeric<T>::value, T>::type> class Vector2;

  template <typename T, bool customStorage>
  class Vector2<T, customStorage, T> : public VectorStorage<T, VECTOR2_COMPONENT_COUNT, customStorage>, public Vector2Components<T> {

  public:

    T& x = this->data[0];
    T& y = this->data[1];

    Vector2(): Vector2(0.0, 0.0) {}
    Vector2(const T& x, const T& y): Vector2Components<T>(this->data, x, y) {}
    Vector2(T* storage): Vector2(storage, 0.0, 0.0) {}
    Vector2(T* storage, const T& x, const T& y):
    VectorStorage<T, VECTOR2_COMPONENT_COUNT, true>(storage), Vector2Components<T>(this->data, x, y) {}

  };

  typedef Vector2<Real, false> Vector2r;
  typedef Vector2<Real, true> Vector2rs;
}
