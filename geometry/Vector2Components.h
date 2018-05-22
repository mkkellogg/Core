#pragma once

#include "../common/types.h"

namespace Core {

  template <typename T>
  class Vector2Components {
  public:
    T& x;
    T& y;

    Vector2Components(T* data, const T& x, const T& y);
    virtual ~Vector2Components() = 0;

    void set(const T& x, const T& y);
    void copy(const Vector2Components<T>& src);

  };

}

#include "Vector2Components.cpp"
