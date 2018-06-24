#pragma once

#include "../math/Matrix4x4.h"

namespace Core {

    // forward declarations
    class Object3D;

    class Transform {
    public:
        enum class TransformationSpace {
            Local = 0,
            World = 1,
            PreLocal = 2
        };

        Transform(const Object3D& target);
        explicit Transform(const Object3D& target, const Matrix4x4& matrix);
        virtual ~Transform();

        Matrix4x4& getLocalMatrix();
        const Matrix4x4& getConstLocalMatrix() const;
        Matrix4x4& getWorldMatrix();
        const Matrix4x4& getConstWorldMatrix() const;

        void toLocalMatrix(Matrix4x4& dest) const;
        void toWorldMatrix(Matrix4x4& dest) const;

        void setTo(const Matrix4x4& mat, Bool updateWorld = false);

        void transform(Vector4<Real>& vector) const;
        void transform(Vector3Base<Real>& vector) const;

        static void getWorldTransformation(WeakPointer<Object3D> target, Matrix4x4& result);
        void getAncestorWorldTransformation(Matrix4x4& result);
        void getWorldTransformation(Matrix4x4& result);
        void updateWorldMatrix();

        void getLocalTransformationFromWorldTransformation(const Matrix4x4& worldTransformation, Matrix4x4& localTransformation);

        void lookAt(const Point3r& target);

        void transformBy(const Matrix4x4& mat, TransformationSpace transformationSpace = TransformationSpace::Local);

        void translate(const Vector3<Real>& dir, TransformationSpace transformationSpace = TransformationSpace::Local);
        void translate(Real x, Real y, Real z, TransformationSpace transformationSpace = TransformationSpace::Local);

    private:
        Matrix4x4 localMatrix;
        Matrix4x4 worldMatrix;
        const Object3D& target;
    };
}
