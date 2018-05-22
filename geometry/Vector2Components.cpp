//
// Created by Mark Kellogg on 1/3/18.
//

#include "Vector2Components.h"

namespace MPM {

  template <typename T>
  Vector2Components<T>::Vector2Components(T* data, const T& x, const T& y): x(data[0]), y(data[1]) {
    this->set(x, y);
  }

  template <typename T>
  Vector2Components<T>::~Vector2Components() {}

  template <typename T>
  void Vector2Components<T>::set(const T& x, const T& y) {
    this->x = x;
    this->y = y;
  }

  template <typename T>
  void Vector2Components<T>::copy(const Vector2Components<T>& src) {
    this->set(src.x, src.y);
  }


}

