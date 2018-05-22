//
// Created by Mark Kellogg on 12/13/17.
//

#ifndef MPM_VECTOR_3_COMPONENTS_H
#define MPM_VECTOR_3_COMPONENTS_H

#include "../common/types.h"
#include "../math/Math.h"

namespace MPM {

  template <typename T>
  class Vector3Components {
  public:
    T& x;
    T& y;
    T& z;

    Vector3Components(T* data, const T& x, const T& y, const T& z): x(data[0]), y(data[1]), z(data[2]) {
      this->set(x, y, z);
    }

    virtual ~Vector3Components() = 0;

    Bool operator == (const Vector3Components<T>& other) {
      if (this == &other) return true;
      Real epsilon = .005f;
      return Math::Abs(other.x - this->x) < epsilon && Math::Abs(other.y - this->y) < epsilon && Math::Abs(other.z - this->z) < epsilon;
    }

    virtual void set(const T& x, const T& y, const T& z) {
      this->x = x;
      this->y = y;
      this->z = z;
    }

    void copy(const Vector3Components<T>& src){
      this->set(src.x, src.y, src.z);
    }

    const Vector3Components& operator =(const Vector3Components& other) {
      if (this == &other) return *this;
      this->copy(other);
    }

  };

  template <typename T>
  Vector3Components<T>::~Vector3Components() {}

}

#endif //MPM_VECTOR_COMPONENTS_3_H
