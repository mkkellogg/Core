#pragma once

#include <iostream>
#include <memory>
#include <type_traits>
#include "../base/VectorStorage.h"
#include "../common/types.h"
#include "Vector3Components.h"

namespace Core {

#define VECTOR3_COMPONENT_COUNT 4

    template <typename T, bool customStorage = false, typename Enable = void>
    class Vector3Base;

    template <typename T, bool customStorage>
    class Vector3Base<T, customStorage, Core::enable_if_t<Core::is_numeric<T>::value>> : public VectorStorage<T, VECTOR3_COMPONENT_COUNT, customStorage>,
                                                                                         public Vector3Components<T> {
    public:
        Vector3Base() : Vector3Base(0.0, 0.0, 0.0) {}
        Vector3Base(const Vector3Base& src) : Vector3Base(src.x, src.y, src.z) {}
        Vector3Base(const T& x, const T& y, const T& z) : VectorStorage<T, VECTOR3_COMPONENT_COUNT, false>(), Vector3Components<T>(this->data, x, y, z) {
            this->set(x, y, z);
        }

        Vector3Base(T* storage) : Vector3Base(storage, 0.0, 0.0, 0.0) {}
        Vector3Base(T* storage, const T& x, const T& y, const T& z)
            : VectorStorage<T, VECTOR3_COMPONENT_COUNT, true>(storage), Vector3Components<T>(this->data, x, y, z) {
            this->set(x, y, z);
        }

        virtual ~Vector3Base() {}

        T getW() const {
            return this->w;
        }

        Vector3Base& operator=(const Vector3Base& other) {
            if (this == &other) return *this;
            this->copy(other);
            return *this;
        }

        operator Vector3Base<T, true>() {
            Vector3Base<T, true> n(this->x, this->y, this->z); 
            n->w = this->w;
            return n;
        }

        virtual void copy(const Vector3Base& src) {
            Vector3Components<T>::copy(src);
        }

        virtual void copy(const Vector3Components<T>& src) override {
            Vector3Components<T>::copy(src);
        }

        void invert() {
            this->scale(-1.0);
        }

        void normalize() {
            Real magnitude = this->magnitude();
            if (magnitude != 0) {
                this->scale(1 / magnitude);
            }
        }

        void scale(Real magnitude) {
            this->x *= magnitude;
            this->y *= magnitude;
            this->z *= magnitude;
        }

        Real magnitude() const {
            return Vector3Base<T, customStorage>::magnitude(this->x, this->y, this->z);
        }

        static Real magnitude(const T& x, const T& y, const T& z) {
            return (Real)Math::squareRoot(x * x + y * y + z * z);
        };

        Bool isZeroLength() const {
            Real epsilon = (Real)1e-06;
            Real sqlen = (this->x * this->x) + (this->y * this->y) + (this->z * this->z);
            return (sqlen < (epsilon * epsilon));
        }

        Real dot(const Vector3Base& other) {
            return Vector3Base::dot(*this, other);
        }

        static Real dot(const Vector3Base& a, const Vector3Base& b) {
            return a.x * b.x + a.y * b.y + a.z * b.z;
        }

        const Vector3Base operator*(const T& scale) const {
            Vector3Base<T, customStorage> vec = *this;
            vec.scale(scale);
            return vec;
        }

        // TODO: optimize this hashing function (implement correctly)
        typedef struct {
            Int32 operator()(const Vector3Base& p) const {
                return (Int32)p.x + (6 << (Int32)p.y) + (12 << (Int32)p.z);
            }

            Int32 operator()(const Vector3Base& p) {
                return (Int32)p.x + (6 << (Int32)p.y) + (12 << (Int32)p.z);
            }
        }Hasher;

        typedef struct {
            Bool operator() (const Vector3Base& a, const Vector3Base& b) const { return a.x == b.x && a.y == b.y && a.z == b.z; }
        }Eq;

    protected:
        T& w = this->data[3];

        const Vector3Base operator-(const Vector3Base<T, customStorage>& other) const {
            Vector3Base<T, customStorage> vec = *this;
            vec.x -= other.x;
            vec.y -= other.y;
            vec.z -= other.z;
            return vec;
        }

        const Vector3Base operator+(const Vector3Base<T, customStorage>& other) const {
            Vector3Base<T, customStorage> vec = *this;
            vec.x += other.x;
            vec.y += other.y;
            vec.z += other.z;
            return vec;
        }
    };

    template <typename T, bool customStorage = false>
    class Vector3 : public Vector3Base<T, customStorage> {
    public:
        static const Vector3<T, customStorage> Zero;
        static const Vector3<T, customStorage> UnitY;
        static const Vector3<T, customStorage> UnitX;
        static const Vector3<T, customStorage> Forward;
        static const Vector3<T, customStorage> Backward;
        static const Vector3<T, customStorage> Left;
        static const Vector3<T, customStorage> Right;
        static const Vector3<T, customStorage> Up;
        static const Vector3<T, customStorage> Down;

        Vector3() : Vector3(0.0, 0.0, 0.0) {}
        Vector3(const Vector3<T, true>& src) : Vector3(src.x, src.y, src.z) {}
        Vector3(const Vector3<T, false>& src) : Vector3(src.x, src.y, src.z) {}
        Vector3(const T& x, const T& y, const T& z) : Vector3Base<T, customStorage>(x, y, z) {
            _set(x, y, z);
        }

        Vector3(T* storage) : Vector3(storage, 0.0, 0.0, 0.0) {}
        Vector3(T* storage, const T& x, const T& y, const T& z) : Vector3Base<T, customStorage>(storage, x, y, z) {
            _set(x, y, z);
        }

        virtual void set(const T& x, const T& y, const T& z) override {
            _set(x, y, z);
        }

        void _set(const T& x, const T& y, const T& z) {
            Vector3Components<T>::set(x, y, z);
            this->w = 0.0;
        }

        static void cross(const Vector3& a, const Vector3& b, Vector3& result) {
            result.x = (a.y * b.z) - (a.z * b.y);
            result.y = (a.z * b.x) - (a.x * b.z);
            result.z = (a.x * b.y) - (a.y * b.x);
        }

        Vector3 operator*(const T& scale) const {
            Vector3Base<T, customStorage> temp = Vector3Base<T, customStorage>::operator*(scale);
            return Vector3(temp.x, temp.y, temp.z);
        }

        Vector3 operator-(const Vector3<T, customStorage>& other) const {
            Vector3Base<T, customStorage> temp = Vector3Base<T, customStorage>::operator-(other);
            return Vector3(temp.x, temp.y, temp.z);
        }

        Vector3 operator+(const Vector3<T, customStorage>& other) const {
            Vector3Base<T, customStorage> temp = Vector3Base<T, customStorage>::operator+(other);
            return Vector3(temp.x, temp.y, temp.z);
        }
    };

    template <typename T, bool customStorage = false>
    class Point3 : public Vector3Base<T, customStorage> {
    public:
        Point3() : Point3(0.0, 0.0, 0.0) {}
        Point3(const Point3<T, true>& src) : Point3(src.x, src.y, src.z) {}
        Point3(const Point3<T, false>& src) : Point3(src.x, src.y, src.z) {}
        Point3(const T& x, const T& y, const T& z) : Vector3Base<T, customStorage>(x, y, z) {
            _set(x, y, z);
        }

        Point3(T* storage) : Point3(storage, 0.0, 0.0, 0.0) {}
        Point3(T* storage, const T& x, const T& y, const T& z) : Vector3Base<T, customStorage>(storage, x, y, z) {
            _set(x, y, z);
        }

        virtual void set(const T& x, const T& y, const T& z) override {
            _set(x, y, z);
        }

        void _set(const T& x, const T& y, const T& z) {
            Vector3Components<T>::set(x, y, z);
            this->w = 1.0;
        }

        Point3 operator*(const T& scale) const {
            Vector3Base<T, customStorage> temp = Vector3Base<T, customStorage>::operator*(scale);
            return Point3(temp.x, temp.y, temp.z);
        }

        Point3 operator-(const Vector3<T, customStorage>& other) const {
            Vector3Base<T, customStorage> temp = Vector3Base<T, customStorage>::operator-(other);
            return Point3(temp.x, temp.y, temp.z);
        }

        Vector3<T, customStorage> operator-(const Point3<T, customStorage>& other) const {
            Vector3Base<T, customStorage> temp = Vector3Base<T, customStorage>::operator-(other);
            return Vector3<T, customStorage>(temp.x, temp.y, temp.z);
        }

        Point3 operator+(const Vector3<T, customStorage>& other) const {
            Vector3Base<T, customStorage> temp = Vector3Base<T, customStorage>::operator+(other);
            return Point3(temp.x, temp.y, temp.z);
        }
    };

    typedef Vector3<Real, false> Vector3r;
    typedef Point3<Real, false> Point3r;
    typedef Vector3<Real, true> Vector3rs;
    typedef Point3<Real, true> Point3rs;

    template <typename T, bool customStorage>
    const Vector3<T, customStorage> Vector3<T, customStorage>::Zero;

    template <typename T, bool customStorage>
    const Vector3<T, customStorage> Vector3<T, customStorage>::UnitY{0.0, 1.0, 0.0};

    template <typename T, bool customStorage>
    const Vector3<T, customStorage> Vector3<T, customStorage>::UnitX{1.0, 0.0, 0.0};

    template <typename T, bool customStorage>
    const Vector3<T, customStorage> Vector3<T, customStorage>::Forward{0.0, 0.0, -1.0};

    template <typename T, bool customStorage>
    const Vector3<T, customStorage> Vector3<T, customStorage>::Backward{0.0, 0.0, 1.0};

    template <typename T, bool customStorage>
    const Vector3<T, customStorage> Vector3<T, customStorage>::Left{-1.0, 0.0, 0.0};

    template <typename T, bool customStorage>
    const Vector3<T, customStorage> Vector3<T, customStorage>::Right{1.0, 0.0, 0.0};

    template <typename T, bool customStorage>
    const Vector3<T, customStorage> Vector3<T, customStorage>::Up{0.0, 1.0, 0.0};

    template <typename T, bool customStorage>
    const Vector3<T, customStorage> Vector3<T, customStorage>::Down{0.0, -1.0, 0.0};
}
