#pragma once

#include "../common/types.h"

namespace Core {

  template <typename T>
  class Vector4Components {
  public:
    T& x;
    T& y;
    T& z;
    T& w;

    Vector4Components(T* data, const T& x, const T& y, const T& z, const T& w): x(data[0]), y(data[1]), z(data[2]), w(data[3]) {
      this->set(x, y, z, w);
    }

    virtual ~Vector4Components() {}

    void set(const T& x, const T& y, const T& z, const T& w) {
      this->x = x;
      this->y = y;
      this->z = z;
      this->w = w;
    }

    void copy(const Vector4Components<T>& src) {
      this->set(src.x, src.y, src.z, src.w);
    }

  };

}
