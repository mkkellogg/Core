#pragma once

#include "../math/Matrix4x4.h"
#include "TransformationSpace.h"

namespace Core {

    // forward declarations
    class Object3D;

    class Transform {
    public:

        Transform(const Object3D& target);
        explicit Transform(const Object3D& target, const Matrix4x4& matrix);
        virtual ~Transform();

        Matrix4x4& getLocalMatrix();
        const Matrix4x4& getConstLocalMatrix() const;
        Matrix4x4& getWorldMatrix();
        const Matrix4x4& getConstWorldMatrix() const;
        Matrix4x4& getInverseWorldMatrix();
        const Matrix4x4& getConstInverseWorldMatrix() const;

        void copyLocalMatrix(Matrix4x4& dest) const;
        void copyWorldMatrix(Matrix4x4& dest) const;
        void setLocalMatrix(const Matrix4x4& mat, Bool updateWorld = false);

        void transform(Vector4<Real>& vector, Bool updateWorldMatrix = true);
        void transform(Vector3Base<Real>& vector, Bool updateWorldMatrix = true);

        void lookAt(const Point3r& target);

        void transformBy(const Matrix4x4& mat, TransformationSpace transformationSpace = TransformationSpace::Local);

        void translate(const Vector3<Real>& dir, TransformationSpace transformationSpace = TransformationSpace::Local);
        void translate(Real x, Real y, Real z, TransformationSpace transformationSpace = TransformationSpace::Local);

        void rotate(const Vector3<Real>& axis, Real angle, TransformationSpace transformationSpace = TransformationSpace::Local);
        void rotate(Real x, Real y, Real z, Real angle, TransformationSpace transformationSpace = TransformationSpace::Local);
        void rotateAround(const Vector3<Real>& axis, const Point3<Real>& pos, Real angle);
        void rotateAround(Real ax, Real ay, Real az, Real px, Real py, Real pz, Real angle);

        void scale(Real x, Real y, Real z);

        void getLocalTransformationFromWorldTransformation(const Matrix4x4& worldTransformation, Matrix4x4& localTransformation);
        void getAncestorWorldTransformation(Matrix4x4& result);
        void getWorldTransformation(Matrix4x4& result);
        void updateWorldMatrix();

        static void getWorldTransformation(WeakPointer<Object3D> target, Matrix4x4& result);

    private:

        Matrix4x4 localMatrix;
        Matrix4x4 worldMatrix;
        Matrix4x4 inverseWorldMatrix; 
        const Object3D& target;
    };
}
