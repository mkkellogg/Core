#pragma once

#include <type_traits>
#include <iostream>
#include <memory>
#include "../common/types.h"
#include "../base/VectorStorage.h"
#include "Vector3Components.h"

namespace Core {

  #define VECTOR3_COMPONENT_COUNT 4

  template <typename T, bool isPoint, bool customStorage, typename Enable = void> class Vector3;

  template <typename T, bool isPoint, bool customStorage>
  class Vector3<T, isPoint, customStorage, Core::enable_if_t<Core::is_numeric<T>::value>>: public VectorStorage<T, VECTOR3_COMPONENT_COUNT, customStorage>, public Vector3Components<T> {

  protected:

    T& w = this->data[3];

  public:

    static const Vector3<T, isPoint, customStorage> Zero;
    static const Vector3<T, isPoint, customStorage> UnitY;
    static const Vector3<T, isPoint, customStorage> UnitX;
    static const Vector3<T, isPoint, customStorage> Forward;

    Vector3() : Vector3(0.0, 0.0, 0.0) {}
    Vector3(const Vector3& src) : Vector3(src.x, src.y, src.z) {}
    Vector3(const T& x, const T& y, const T& z):
        VectorStorage<T, VECTOR3_COMPONENT_COUNT, false>(),
        Vector3Components<T>(this->data, x, y, z) {
      this->set(x, y, z);
    }

    Vector3(T* storage): Vector3(storage, 0.0, 0.0, 0.0) {}
    Vector3(T* storage, const T& x, const T& y, const T& z):
        VectorStorage<T, VECTOR3_COMPONENT_COUNT, true>(storage), Vector3Components<T>(this->data, x, y, z) {
      this->set(x, y, z);
    }

    const Vector3& operator =(const Vector3& other){
      if (this == &other) return *this;
      this->copy(other);
    }

    void set(const T& x, const T& y, const T& z) override {
      Vector3Components<T>::set(x, y, z);
      this->w = isPoint ? 1.0 : 0.0;
    }

    void normalize() {
      Real magnitude = this->magnitude();
      if (magnitude != 0)
      {
        this->scale(1 / magnitude);
      }
    }

    void scale(Real magnitude) {
      this->x *= magnitude;
      this->y *= magnitude;
      this->z *= magnitude;
    }

    Real magnitude() const {
      return Vector3<T, isPoint, customStorage>::magnitude(this->x, this->y, this->z);
    }

    Bool isZeroLength() const {
      Real epsilon = (Real)1e-06;
      Real sqlen = (this->x * this->x) + (this->y * this->y) + (this->z * this->z);
      return (sqlen < (epsilon * epsilon));
    }

    static Real dot(const Vector3& a, const Vector3& b) {
      return a.x * b.x + a.y * b.y + a.z * b.z;
    }

    static void cross(const Vector3& a, const Vector3& b, Vector3& result)  {
      result.x = (a.y * b.z) - (b.y * a.z);
      result.y = (b.x * a.z) - (a.x * b.z);
      result.z = (a.x * b.y) - (b.x * a.y);
    }

    static Real magnitude(const T& x, const T& y, const T& z) {
      return (Real)Math::SquareRoot(x * x + y * y + z * z);
    };

    const Vector3 operator -(const Vector3Components<T>& other) const {
      Vector3<T, isPoint, customStorage> vec = *this;
      vec.x -= other.x;
      vec.y -= other.y;
      vec.z -= other.z;
      return vec;
    }

   // template <typename TV, bool isPointV, bool customStorageV>
    //void testFunc(const Vector3<TV, isPointV, customStorageV>& other);

    /*template <typename TV, bool isPointV, bool customStorageV>
    const Vector3 operator -(const Vector3<TV, isPointV, customStorageV>& other) const {
      Vector3<T, isPoint, customStorage> vec = *this;
      vec.x -= other.x;
      vec.y -= other.y;
      vec.z -= other.z;
      return vec;
    }*/

    const Vector3 operator *(const T& scale) const {
      Vector3<T, isPoint, customStorage> vec = *this;
      vec.x *= scale;
      vec.y *= scale;
      vec.z *= scale;
      return vec;

    }

  };


  /*template <typename T, bool isPoint, bool customStorage>
  template <typename TV, bool isPointV, bool customStorageV>
  void Vector3<T, isPoint, customStorage, T>::testFunc(const Vector3<TV, isPointV, customStorageV>& other) {

  }*/



  typedef Vector3<Real, false, false> Vector3r;
  typedef Vector3<Real, true, false> Point3r;
  typedef Vector3<Real, false, true> Vector3rs;
  typedef Vector3<Real, true, true> Point3rs;

 
  template <typename T, bool isPoint, bool customStorage>
  const Vector3<T, isPoint, customStorage> Vector3<T, isPoint, customStorage, Core::enable_if_t<Core::is_numeric<T>::value>>::Zero;

  template <typename T, bool isPoint, bool customStorage>
  const Vector3<T, isPoint, customStorage> Vector3<T, isPoint, customStorage, Core::enable_if_t<Core::is_numeric<T>::value>>::UnitY {0.0, 1.0, 0.0};

  template <typename T, bool isPoint, bool customStorage>
  const Vector3<T, isPoint, customStorage> Vector3<T, isPoint, customStorage, Core::enable_if_t<Core::is_numeric<T>::value>>::UnitX {1.0, 0.0, 0.0};

  template <typename T, bool isPoint, bool customStorage>
  const Vector3<T, isPoint, customStorage> Vector3<T, isPoint, customStorage, Core::enable_if_t<Core::is_numeric<T>::value>>::Forward {0.0, 0.0, -1.0};


}
