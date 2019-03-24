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
        void setLocalMatrix(const Matrix4x4& mat);

        void applyTransformationTo(Vector4<Real>& vector);
        void applyTransformationTo(Vector3Base<Real>& vector);

        void lookAt(const Point3r& target);
        void lookAt(const Point3r& target, const Vector3r& up);

        void transformBy(const Matrix4x4& mat, TransformationSpace transformationSpace = TransformationSpace::Local);

        void translate(const Vector3<Real>& dir, TransformationSpace transformationSpace = TransformationSpace::Local);
        void translate(Real x, Real y, Real z, TransformationSpace transformationSpace = TransformationSpace::Local);

        void rotate(const Vector3<Real>& axis, Real angle, TransformationSpace transformationSpace = TransformationSpace::Local);
        void rotate(Real x, Real y, Real z, Real angle, TransformationSpace transformationSpace = TransformationSpace::Local);
        void rotateAround(const Vector3<Real>& axis, const Point3<Real>& pos, Real angle);
        void rotateAround(Real ax, Real ay, Real az, Real px, Real py, Real pz, Real angle);

        void scale(Real x, Real y, Real z);

        void setWorldPosition(const Vector3Base<Real>& position);
        void setWorldPosition(const Vector3<Real>& position);
        void setWorldPosition(Real x, Real y, Real z);
        Point3r getWorldPosition();

        void updateWorldMatrix();

    private:

        void getAncestorWorldTransformation(Matrix4x4& result);
        void getWorldTransformation(Matrix4x4& result);
        void getLocalTransformationFromWorldTransformation(const Matrix4x4& newWorldTransformation, Matrix4x4& localTransformation);
        void getLocalTransformationFromWorldTransformation(const Matrix4x4& newWorldTransformation, const Matrix4x4& currentFullTransformation, Matrix4x4& localTransformation);
        static void getWorldTransformation(WeakPointer<Object3D> target, Matrix4x4& result);

        Matrix4x4 localMatrix;
        Matrix4x4 worldMatrix;
        Matrix4x4 inverseWorldMatrix; 
        const Object3D& target;
    };
}
