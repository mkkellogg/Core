//
// Created by Mark Kellogg on 12/16/17.
//

#include "string.h"
#include "Camera.h"
#include "../common/types.h"
#include "../math/Math.h"
#include "../math/Matrix4x4.h"

namespace Core {

  const UInt32 Camera::DEFAULT_FOV = 70;
  const UInt32 Camera::DEFAULT_WIDTH = 1200;
  const UInt32 Camera::DEFAULT_HEIGHT = 800;
  const Real Camera::DEFAULT_RATIO = (Real)Camera::DEFAULT_WIDTH / (Real)Camera::DEFAULT_HEIGHT;
  const Real Camera::DEFAULT_NEARP = 0.1;
  const Real Camera::DEFAULT_FARP = 100.0;

  Camera::Camera(): fov(Camera::DEFAULT_FOV), aspectRatio(Camera::DEFAULT_RATIO),
                    nearP(Camera::DEFAULT_NEARP), farP(Camera::DEFAULT_FARP) {
    this->updateProjection(this->fov, this->aspectRatio, this->nearP, this->farP);
  }

  Camera::Camera(Real fov, Real ratio, Real nearP, Real farP):
      fov(fov), aspectRatio(ratio), nearP(nearP), farP(farP) {
    this->updateProjection(this->fov, this->aspectRatio, this->nearP, this->farP);
  }

  void Camera::updateProjection(Real fov, Real ratio, Real nearP, Real farP) {
    Camera::buildPerspectiveProjectionMatrix(fov, ratio, nearP, farP, this->projectionMatrix);
  }

  void Camera::setAspectRatio(Real ratio) {
    this->aspectRatio = ratio;
    this->updateProjection(this->fov, this->aspectRatio, this->nearP, this->farP);
  }

  void Camera::setAspectRatioFromDimensions(UInt32 width, UInt32 height) {
    this->aspectRatio = (Real)width / (Real)height;
    this->updateProjection(this->fov, this->aspectRatio, this->nearP, this->farP);
  }

  const Matrix4x4& Camera::getProjectionMatrix() {
    return this->projectionMatrix;
  }

  void Camera::buildPerspectiveProjectionMatrix(Real fov, Real ratio,
                                                Real nearP, Real farP, Matrix4x4& out)
  {
    // convert fov to radians
    Real f = 1.0f / Math::Tan(fov * Math::DegreesToRads * .5f);

    Real data[16];
    memset(data, 0, 16 * sizeof(Real));

    data[0] = f / ratio;
    data[1 * 4 + 1] = f;
    data[2 * 4 + 2] = (farP + nearP) / (nearP - farP);
    data[3 * 4 + 2] = (2.0f * farP * nearP) / (nearP - farP);
    data[2 * 4 + 3] = -1.0f;
    data[3 * 4 + 3] = 0.0f;
    out.copy(data);
  }

}
