#pragma once

#include "../common/types.h"

namespace Core {

  template <typename T>
  class Vector2Components {
  public:
    T& x;
    T& y;

    Vector2Components(T* data, const T& x, const T& y): x(data[0]), y(data[1]) {
      this->set(x, y);
    }

    virtual ~Vector2Components() {}

    void set(const T& x, const T& y) {
      this->x = x;
      this->y = y;
    }

    void copy(const Vector2Components<T>& src) {
      this->set(src.x, src.y);
    }

  };

}
