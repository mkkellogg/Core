#pragma once

#include "../common/types.h"
#include "../geometry/Vector3.h"
#include "Matrix4x4.h"

namespace Core {
    class Quaternion {
        Real mData[4];

    public:
        static const Quaternion Identity;

        Quaternion();
        explicit Quaternion(const Matrix4x4& matrix);
        Quaternion(const Vector3Components<Real>& v, Real w);
        explicit Quaternion(const Real* array);
        Quaternion(Real x, Real y, Real z, Real w);
        void set(Real x, Real y, Real z, Real w);
        Real x() const;
        Real y() const;
        Real z() const;
        Real w() const;
        Vector3r complex() const;
        void complex(const Vector3Components<Real>& c);
        Real real() const;
        void real(Real r);
        Quaternion conjugate(void) const;
        Quaternion inverse(void) const;
        Quaternion product(const Quaternion& rhs) const;
        Quaternion operator*(const Quaternion& rhs) const;
        Quaternion operator*(Real s) const;
        Quaternion operator+(const Quaternion& rhs) const;
        Quaternion operator-(const Quaternion& rhs) const;
        Quaternion operator-() const;
        Quaternion operator/(Real s) const;
        Matrix4x4 matrix() const;
        Matrix4x4 rightMatrix() const;
        Vector3r vector() const;
        void normalize();
        Real norm() const;
        void setRotationMatrix(Real* data) const;
        Matrix4x4 rotationMatrix() const;
        void rotationMatrix(Matrix4x4& out) const;
        void fromMatrix(const Matrix4x4& matrix);
        void fromAngleAxis(const Real rfAngle, const Vector3Components<Real>& rkAxis);
        void fromAngleAxis(Real angle, Real x, Real y, Real z);
        // void scaledAxis(const Vector3Components<Real>& w);
        Vector3r rotatedVector(const Vector3Components<Real>& v) const;
        void euler(const Vector3Components<Real>& euler);
        Vector3r euler(void) const;
        // void decoupleZ(Quaternion* Qxy, Quaternion* Qz) const;
        Quaternion slerp(const Quaternion& q1, Real t);
        static Quaternion slerp(const Quaternion& Qa, const Quaternion& Qb, const Real t);
        Real* row(UInt32 i);
        const Real* row(UInt32 i) const;
        static Quaternion getRotation(const Vector3Components<Real>& source, const Vector3Components<Real>& dest);
        static Quaternion getRotation(const Vector3Components<Real>& source, const Vector3Components<Real>& dest, const Vector3Components<Real>& fallbackAxis);
    };

    // Global operator allowing left-multiply by scalar.
    Quaternion operator*(Real s, const Quaternion& q);
}
