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

    virtual ~Vector2Components() = 0;

    void set(const T& x, const T& y) {
      this->x = x;
      this->y = y;
    }

  };

  template <typename T>
  Vector2Components<T>::~Vector2Components() {}
}
