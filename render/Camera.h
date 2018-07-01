#pragma once

#include "../common/types.h"
#include "../geometry/Vector3.h"
#include "../math/Matrix4x4.h"
#include "../scene/Object3DComponent.h"

namespace Core {

    // forward declarations
    class Engine;

    class Camera : public Object3DComponent {
        friend class Engine;

    public:
        static const UInt32 DEFAULT_FOV;
        static const UInt32 DEFAULT_WIDTH;
        static const UInt32 DEFAULT_HEIGHT;
        static const Real DEFAULT_RATIO;
        static const Real DEFAULT_NEARP;
        static const Real DEFAULT_FARP;

        void setAspectRatio(Real ratio);
        void setAspectRatioFromDimensions(UInt32 width, UInt32 height);
        void updateProjection(Real fov, Real ratio, Real nearP, Real farP);
        const Matrix4x4& getProjectionMatrix() const;
        void updateWorlInverseTransposeMatrix() ;
        const Matrix4x4& getWorlInverseTransposeMatrix() const;
        void lookAt(const Point3r& target);
        void project(Vector3Base<Real>& vec);
        void unProject(Vector3Base<Real>& vec);

        static void buildPerspectiveProjectionMatrix(Real fov, Real ratio, Real nearP, Real farP, Matrix4x4& out);

    private:
        Camera(WeakPointer<Object3D> owner);
        Camera(WeakPointer<Object3D> owner, Real fov, Real ratio, Real nearP, Real farP);

        Real fov;
        Real aspectRatio;
        Real nearP;
        Real farP;
        Matrix4x4 projectionMatrix;
        Matrix4x4 worldInverseTransposeMatrix;
    };
}
