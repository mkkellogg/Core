#pragma once

#include "../common/types.h"
#include "../math/Matrix4x4.h"
#include "../scene/Object3D.h"

namespace Core {
  class Camera : public Object3D {
    Real fov;
    Real aspectRatio;
    Real nearP;
    Real farP;
    Matrix4x4 projectionMatrix;
  public:

    static const UInt32 DEFAULT_FOV;
    static const UInt32 DEFAULT_WIDTH;
    static const UInt32 DEFAULT_HEIGHT;
    static const Real DEFAULT_RATIO;
    static const Real DEFAULT_NEARP;
    static const Real DEFAULT_FARP;

    Camera();
    Camera(Real fov, Real ratio, Real nearP, Real farP);
    void setAspectRatio(Real ratio);
    void setAspectRatioFromDimensions(UInt32 width, UInt32 height);
    void updateProjection(Real fov, Real ratio, Real nearP, Real farP);
    const Matrix4x4& getProjectionMatrix();

    static void buildPerspectiveProjectionMatrix(Real fov, Real ratio,
                                                 Real nearP, Real farP, Matrix4x4& out);
  };
}
