#pragma once

#include "../base/BaseVector.h"
#include "../common/types.h"
#include "../geometry/Vector3.h"
#include "../geometry/Vector4.h"

namespace Core {

    // forward declarations
    class Quaternion;

#define SIZE_MATRIX_4X4 16
#define ROWSIZE_MATRIX_4X4 4

    class Matrix4x4 {
    public:
        Matrix4x4();
        explicit Matrix4x4(const Real* sourceData);
        Matrix4x4(const Matrix4x4& source);
        virtual ~Matrix4x4();

        Real& A0 = data[0];
        Real& A1 = data[4];
        Real& A2 = data[8];
        Real& A3 = data[12];
        Real& B0 = data[1];
        Real& B1 = data[5];
        Real& B2 = data[9];
        Real& B3 = data[13];
        Real& C0 = data[2];
        Real& C1 = data[6];
        Real& C2 = data[10];
        Real& C3 = data[14];
        Real& D0 = data[3];
        Real& D1 = data[7];
        Real& D2 = data[11];
        Real& D3 = data[15];

        Real* getData();
        const Real* getConstData() const;
        void setIdentity();
        void setIdentity(Real* target);

        Matrix4x4& operator=(const Matrix4x4& source);
        void copy(const Matrix4x4& src);
        void copy(const Real* sourceData);

        void transpose();
        static void transpose(const Real* source, Real* dest);
        Bool invert();
        Bool invert(Matrix4x4& out);
        static Bool invert(const Real* source, Real* dest);

        void buildFromComponents(const Vector3Components<Real>& translation, const Quaternion& rotation, const Vector3Components<Real>& scale);
        void decompose(Vector3Components<Real>& translation, Quaternion& rotation, Vector3Components<Real>& scale) const;
        Bool isAffine(void) const;
        static Bool isAffine(const Real* data);

        Real calculateDeterminant(Real* adjoinOut = nullptr) const;
        static Real calculateDeterminant(const Real* source, Real* adjoinOut = nullptr);

        void multiplyByScalar(Real scalar);
        void transform(const Vector4<Real>& vector, Vector4<Real>& out) const;
        void transform(Vector4<Real>& vector) const;
        void transform(const Vector3Base<Real>& vector, Vector3Base<Real>& out) const;
        void transform(Vector3Base<Real>& vector) const;
        void transform(Real* vector4f) const;
        void add(const Matrix4x4& matrix);
        void multiply(const Matrix4x4& matrix);
        void preMultiply(const Matrix4x4& matrix);
        void multiply(const Matrix4x4& matrix, Matrix4x4& out) const;
        static void multiply(const Matrix4x4& lhs, const Matrix4x4& rhs, Matrix4x4& out);
        static void multiplyMV(const Real* lhsMat, const Real* rhsVec, Real* out);
        static inline void mx4transform(Real x, Real y, Real z, Real w, const Real* matrix, Real* pDest);
        static void multiplyMM(const Real* lhs, const Real* rhs, Real* out);

        void translate(const Vector3Components<Real>& vector);
        void translate(Real x, Real y, Real z);
        void preTranslate(const Vector3Components<Real>& vector);
        void preTranslate(Real x, Real y, Real z);
        void setTranslation(Real x, Real y, Real z);
        Vector3r getTranslation();
        static void translate(const Matrix4x4& src, Matrix4x4& out, const Vector3Components<Real>& vector);
        static void translate(const Matrix4x4& src, Matrix4x4& out, Real x, Real y, Real z);
        static void translate(const Real* source, Real* dest, Real x, Real y, Real z);
        static void preTranslate(const Real* source, Real* dest, Real x, Real y, Real z);

        void rotate(const Vector3Components<Real>& vector, Real a);
        void rotate(Real x, Real y, Real z, Real a);
        void preRotate(const Vector3Components<Real>& vector, Real a);
        void preRotate(Real x, Real y, Real z, Real a);
        void makeRotationFromEuler(Real x, Real y, Real z);
        Vector3r getRotationAsEuler();
        void setRotationFromEuler(Real x, Real y, Real z);
        void setRotationFromEuler(const Vector3<Real>& euler);
        void setRotationFromQuaternion(const Quaternion& quaternion);
        static void makeRotation(Real* rm, Real x, Real y, Real z, Real a);
        static void makeRotation(Matrix4x4& m, Real x, Real y, Real z, Real a);
        static void makeRotationFromEuler(Real* rm, Real x, Real y, Real z);
        static void makeRotationFromEuler(Matrix4x4& m, Real x, Real y, Real z);

        void scale(const Vector3Components<Real>& scale);
        void scale(Real x, Real y, Real z);
        void preScale(Real x, Real y, Real z);
        void scale(Matrix4x4& out, Real x, Real y, Real z) const;
        Vector3r getScale();
        void setScale(const Vector3Components<Real>& scale);
        void setScale(Real x, Real y, Real z);
        static void scale(const Real* source, Real* dest, Real x, Real y, Real z);
        static void preScale(const Real* source, Real* dest, Real x, Real y, Real z);
        static void makeScale(Matrix4x4& m, Real x, Real y, Real z);
        static void makeScale(Matrix4x4& m, const Vector3Components<Real>& vector);

        void lookAt(const Vector3Components<Real>& src, const Vector3Components<Real>& target, const Vector3Components<Real>& up);

    private:
        Real data[SIZE_MATRIX_4X4];
        static Vector3r zero;
        static Vector3r one;
    };
}
