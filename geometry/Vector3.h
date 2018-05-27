#pragma once

#include <type_traits>
#include <iostream>
#include <memory>
#include "../common/types.h"
#include "../base/VectorStorage.h"
#include "Vector3Components.h"

namespace Core {

  #define VECTOR3_COMPONENT_COUNT 4

  template <typename T, bool customStorage, bool isPoint, typename Enable = void> class Vector3;

  template <typename T, bool customStorage, bool isPoint>
  class Vector3<T, customStorage, isPoint, Core::enable_if_t<Core::is_numeric<T>::value>>: public VectorStorage<T, VECTOR3_COMPONENT_COUNT, customStorage>, public Vector3Components<T> {

  protected:

    T& w = this->data[3];

  public:

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
      return Vector3<T, customStorage, isPoint>::magnitude(this->x, this->y, this->z);
    }

    Bool isZeroLength() const {
      Real epsilon = (Real)1e-06;
      Real sqlen = (this->x * this->x) + (this->y * this->y) + (this->z * this->z);
      return (sqlen < (epsilon * epsilon));
    }

    static Real dot(const Vector3& a, const Vector3& b) {
      return a.x * b.x + a.y * b.y + a.z * b.z;
    }

    static Real magnitude(const T& x, const T& y, const T& z) {
      return (Real)Math::SquareRoot(x * x + y * y + z * z);
    };

    const Vector3 operator *(const T& scale) const {
      Vector3<T, customStorage, isPoint> vec = *this;
      vec.scale(scale);
      return vec;

    }

  protected:

    const Vector3 operator -(const Vector3<T, customStorage, isPoint>& other) const {
      Vector3<T, customStorage, isPoint> vec = *this;
      vec.x -= other.x;
      vec.y -= other.y;
      vec.z -= other.z;
      return vec;
    }

    const Vector3 operator +(const Vector3<T, customStorage, isPoint>& other) const {
      Vector3<T, customStorage, isPoint> vec = *this;
      vec.x += other.x;
      vec.y += other.y;
      vec.z += other.z;
      return vec;
    }

  };

  
  template <typename T, bool customStorage>
  class Vector3v : public Vector3<T, customStorage, false> {

  public:
    static const Vector3v<T, customStorage> Zero;
    static const Vector3v<T, customStorage> UnitY;
    static const Vector3v<T, customStorage> UnitX;
    static const Vector3v<T, customStorage> Forward;

    Vector3v() : Vector3v(0.0, 0.0, 0.0) {}
    Vector3v(const Vector3<T, customStorage, false>& src) : Vector3v(src.x, src.y, src.z) {}
    Vector3v(const T& x, const T& y, const T& z): Vector3<T, customStorage, false>(x, y, z) {
    }

    Vector3v(T* storage): Vector3v(storage, 0.0, 0.0, 0.0) {}
    Vector3v(T* storage, const T& x, const T& y, const T& z): Vector3<T, customStorage, false>(storage, x, y, z) {
    }

    static void cross(const Vector3v& a, const Vector3v& b, Vector3v& result)  {
      result.x = (a.y * b.z) - (b.y * a.z);
      result.y = (b.x * a.z) - (a.x * b.z);
      result.z = (a.x * b.y) - (b.x * a.y);
    }

    Vector3v operator *(const T& scale) const {
      return Vector3v(Vector3<T, customStorage, false>::operator * (scale));
    }

    Vector3v operator -(const Vector3v<T, customStorage>& other) const {
      return Vector3v(Vector3<T, customStorage, false>::operator -(other));
    }

    Vector3v operator +(const Vector3v<T, customStorage>& other) const {
      return Vector3v(Vector3<T, customStorage, false>::operator +(other));
    }

  };

  template <typename T, bool customStorage>
  class Vector3p : public Vector3<T, customStorage, true> {
  
  public:
    Vector3p() : Vector3p(0.0, 0.0, 0.0) {}
    Vector3p(const Vector3<T, customStorage, true>& src) : Vector3p(src.x, src.y, src.z) {}
    Vector3p(const T& x, const T& y, const T& z): Vector3<T, customStorage, true>(x, y, z) {
    }

    Vector3p(T* storage): Vector3p(storage, 0.0, 0.0, 0.0) {}
    Vector3p(T* storage, const T& x, const T& y, const T& z): Vector3<T, customStorage, true>(storage, x, y, z) {
    }

    Vector3p operator *(const T& scale) const {
      return Vector3p(Vector3<T, customStorage, false>::operator * (scale));
    }

    Vector3p operator -(const Vector3v<T, customStorage>& other) const {
      return Vector3p(Vector3<T, customStorage, true>::operator -(other));
    }

    Vector3v<T, customStorage> operator -(const Vector3p<T, customStorage>& other) const {
      auto res = Vector3<T, customStorage, true>::operator -(other);
      return Vector3v<T, customStorage>(res.x, res.y, res.z);
    }

    Vector3p operator +(const Vector3v<T, customStorage>& other) const {
      return Vector3p(Vector3<T, customStorage, true>::operator +(other));
    }

  };

  typedef Vector3v<Real, false> Vector3r;
  typedef Vector3p<Real, false> Point3r;
  typedef Vector3v<Real, true> Vector3rs;
  typedef Vector3p<Real, true> Point3rs;

  template <typename T, bool customStorage>
  const Vector3v<T, customStorage> Vector3v<T, customStorage>::Zero;

  template <typename T, bool customStorage>
  const Vector3v<T, customStorage> Vector3v<T, customStorage>::UnitY {0.0, 1.0, 0.0};

  template <typename T, bool customStorage>
  const Vector3v<T, customStorage> Vector3v<T, customStorage>::UnitX {1.0, 0.0, 0.0};

  template <typename T, bool customStorage>
  const Vector3v<T, customStorage> Vector3v<T, customStorage>::Forward {0.0, 0.0, -1.0};

}
