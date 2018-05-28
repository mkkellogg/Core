#pragma once

#include "../common/types.h"
#include "../base/VectorStorage.h"
#include "Vector4Components.h"

namespace Core {

  #define VECTOR4_COMPONENT_COUNT 4

  template <typename T, bool customStorage,
      typename = typename std::enable_if<Core::is_numeric<T>::value, T>::type> class Vector4;

  template <typename T, bool customStorage>
  class Vector4<T, customStorage, T> : public VectorStorage<T, VECTOR4_COMPONENT_COUNT, customStorage>, public Vector4Components<T> {

  public:

    T& x = this->data[0];
    T& y = this->data[1];
    T& z = this->data[0];
    T& w = this->data[1];

    Vector4(): Vector4(0.0, 0.0) {}
    Vector4(const T& x, const T& y): Vector4Components<T>(this->data, x, y, z, w) {}
    Vector4(T* storage): Vector4(storage, 0.0, 0.0) {}
    Vector4(T* storage, const T& x, const T& y, const T& z, const T& w):
    VectorStorage<T, VECTOR4_COMPONENT_COUNT, true>(storage), Vector4Components<T>(this->data, x, y, z, w) {}

  };

  typedef Vector4<Real, false> Vector4r;
  typedef Vector4<Real, true> Vector4rs;

  typedef Vector4<Int32, false> Vector4i;
  typedef Vector4<Int32, true> Vector4is;

  typedef Vector4<UInt32, false> Vector4u;
  typedef Vector4<UInt32, true> Vector4us;
}
