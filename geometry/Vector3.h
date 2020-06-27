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

        operator Vector3Base<T, true>() {
            Vector3Base<T, true> n(this->x, this->y, this->z); 
            n->w = this->w;
            return n;
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

        T magnitude() const {
            return Vector3Base<T, customStorage>::magnitude(this->x, this->y, this->z);
        }

        static T magnitude(const T& x, const T& y, const T& z) {
            return Math::squareRoot(x * x + y * y + z * z);
        };

        Real squareMagnitude() const {
            return Vector3Base<T, customStorage>::squareMagnitude(this->x, this->y, this->z);
        }

        static T squareMagnitude(const T& x, const T& y, const T& z) {
            return x * x + y * y + z * z;
        };

        Bool isZeroLength() const {
            Real epsilon = (Real)1e-06;
            Real sqlen = (this->x * this->x) + (this->y * this->y) + (this->z * this->z);
            return (sqlen < (epsilon * epsilon));
        }

        Real dot(const Vector3Base& other) {
            return Vector3Base::dot(*this, other);
        }

        static Real dot(const Vector3Components<Real>& a, const Vector3Components<Real>& b) {
            return a.x * b.x + a.y * b.y + a.z * b.z;
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
            Bool operator() (const Vector3Base& a, const Vector3Base& b) const {
                Real epsilon = .005f;
                return Math::abs(a.x - b.x) < epsilon && Math::abs(a.y - b.y) < epsilon && Math::abs(a.z - b.z) < epsilon;
    
            }
        }Eq;

    protected:
        T& w = this->data[3];

        template <Bool otherCustomStorage>
        Bool operator==(const Vector3Base<T, otherCustomStorage>& other) {
            if (this == &other) return true;
            Real epsilon = .005f;
            return Math::abs(other.x - this->x) < epsilon && Math::abs(other.y - this->y) < epsilon && Math::abs(other.z - this->z) < epsilon;
        }

        const Vector3Base<T, false> operator*(Real scale) const {
            Vector3Base<T, false> vec = *this;
            vec.scale(scale);
            return vec;
        }

        template <Bool otherCustomStorage>
        void subtract(const Vector3Base<T, otherCustomStorage>& other) {
            this->x -= other.x;
            this->y -= other.y;
            this->z -= other.z;
        }

        template <Bool otherCustomStorage>
        void add(const Vector3Base<T, otherCustomStorage>& other) {
            this->x += other.x;
            this->y += other.y;
            this->z += other.z;
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

        Vector3& operator=(const Vector3& other) {
            if (this == &other) return *this;
            this->copy(other);
            return *this;
        }

        template <Bool otherCustomStorage>
        Vector3& operator=(const Vector3<T, otherCustomStorage>& other) {
            this->copy(other);
            return *this;
        }

        template <Bool otherCustomStorage>
        void copy(const Vector3<T, otherCustomStorage>& other) {
            BaseVector<T, VECTOR3_COMPONENT_COUNT>::copy(other);
        }

        template <Bool otherCustomStorage>
        Bool operator==(const Vector3<T, otherCustomStorage>& other) {
            return Vector3Base<T, customStorage>::operator==(other);
        }

        template <Bool otherCustomStorage>
        Vector3<T, false> operator-(const Vector3<T, otherCustomStorage>& other) const {
            Vector3<T, false> temp(this->x, this->y, this->z);
            temp.subtract(other);
            return temp;
        }

        template <Bool otherCustomStorage>
        Vector3<T, false> operator+(const Vector3<T, otherCustomStorage>& other) const {
            Vector3<T, false> temp(this->x, this->y, this->z);
            temp.add(other);
            return temp;
        }

        Vector3<T, false> operator*(const T& scale) const {
            Vector3<T, false> vec = *this;
            vec.scale(scale);
            return vec;
        }

        template <Bool otherCustomStorage>
        Vector3<T, false> cross(const Vector3<T, otherCustomStorage>& other) const {
            Vector3<T, false> result;
            cross(*this, other, result);
            return result;
        }

        static void cross(const Vector3& a, const Vector3& b, Vector3& result) {
            result.x = (a.y * b.z) - (a.z * b.y);
            result.y = (a.z * b.x) - (a.x * b.z);
            result.z = (a.x * b.y) - (a.y * b.x);
        }
    
    private:

        void _set(const T& x, const T& y, const T& z) {
            Vector3Components<T>::set(x, y, z);
            this->w = 0.0;
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

        Point3& operator=(const Point3& other) {
            if (this == &other) return *this;
            this->copy(other);
            return *this;
        }

        template <Bool otherCustomStorage>
        Point3& operator=(const Point3<T, otherCustomStorage>& other) {
            this->copy(other);
            return *this;
        }

        template <Bool otherCustomStorage>
        void copy(const Point3<T, otherCustomStorage>& other) {
            BaseVector<T, VECTOR3_COMPONENT_COUNT>::copy(other);
        }

        template <Bool otherCustomStorage>
        Bool operator==(const Point3<T, otherCustomStorage>& other) {
            return Vector3Base<T, customStorage>::operator==(other);
        }

        Point3<T, false> operator*(const T& scale) const {
            Vector3Base<T, false> temp = Vector3Base<T, customStorage>::operator*(scale);
            return Point3<T, false>(temp.x, temp.y, temp.z);
        }

        template <Bool otherCustomStorage>
        Point3<T, false> operator-(const Vector3<T, otherCustomStorage>& other) const {
            Point3<T, false> temp(this->x, this->y, this->z);
            temp.subtract(other);
            return temp;
        }

        template <Bool otherCustomStorage>
        Vector3<T, false> operator-(const Point3<T, otherCustomStorage>& other) const {
            Vector3<T, false> temp(this->x, this->y, this->z);
            Vector3<T, false> othertemp(other.x, other.y, other.z);
            return temp - othertemp;
        }

        template <Bool otherCustomStorage>
        Point3<T, false> operator+(const Vector3<T, otherCustomStorage>& other) const {
            Point3<T, false> temp(this->x, this->y, this->z);
            temp.add(other);
            return temp;
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
