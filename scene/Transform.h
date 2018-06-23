#pragma once

#include "../math/Matrix4x4.h"

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

        void toLocalMatrix(Matrix4x4& dest) const;
        void toWorldMatrix(Matrix4x4& dest) const;

        void setTo(const Matrix4x4& mat, Bool updateWorld = false);

        void updateWorldMatrix();

        void transform(Vector4<Real>& vector) const;
        void transform(Vector3Base<Real>& vector, Bool asPoint) const;

        void lookAt(const Point3r& target);

    private:
        Matrix4x4 localMatrix;
        Matrix4x4 worldMatrix;
        const Object3D& target;
    };
}
