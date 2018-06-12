#include "Camera.h"
#include "../common/types.h"
#include "../math/Math.h"
#include "../math/Matrix4x4.h"
#include "../math/Quaternion.h"
#include "../util/ValidWeakPointer.h"
#include "string.h"

namespace Core {

    const UInt32 Camera::DEFAULT_FOV = 70;
    const UInt32 Camera::DEFAULT_WIDTH = 1200;
    const UInt32 Camera::DEFAULT_HEIGHT = 800;
    const Real Camera::DEFAULT_RATIO = (Real)Camera::DEFAULT_WIDTH / (Real)Camera::DEFAULT_HEIGHT;
    const Real Camera::DEFAULT_NEARP = 0.1;
    const Real Camera::DEFAULT_FARP = 100.0;

    Camera::Camera() : fov(Camera::DEFAULT_FOV), aspectRatio(Camera::DEFAULT_RATIO), nearP(Camera::DEFAULT_NEARP), farP(Camera::DEFAULT_FARP) {
        this->updateProjection(this->fov, this->aspectRatio, this->nearP, this->farP);
    }

    Camera::Camera(Real fov, Real ratio, Real nearP, Real farP) : fov(fov), aspectRatio(ratio), nearP(nearP), farP(farP) {
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

    const Matrix4x4& Camera::getProjectionMatrix() const {
        return this->projectionMatrix;
    }

    void Camera::lookAt(const Point3r& target) {
        Point3r cameraPos;
        this->transform.updateWorldMatrix();
        this->transform.transform(cameraPos, true);

        Vector3r toTarget = target - cameraPos;
        toTarget.normalize();

        Vector3r vUp(0, 1, 0);
        Vector3r vRight;

        Vector3r::cross(toTarget, vUp, vRight);
        vRight.normalize();

        Vector3r::cross(vRight, toTarget, vUp);
        vUp.normalize();

        Matrix4x4 full = this->transform.getLocalMatrix();
        auto fullMat = full.getData();

        fullMat[0] = vRight.x;
        fullMat[1] = vRight.y;
        fullMat[2] = vRight.z;
        fullMat[3] = 0.0f;

        fullMat[4] = vUp.x;
        fullMat[5] = vUp.y;
        fullMat[6] = vUp.z;
        fullMat[7] = 0.0f;

        fullMat[8] = -toTarget.x;
        fullMat[9] = -toTarget.y;
        fullMat[10] = -toTarget.z;
        fullMat[11] = 0.0f;

        fullMat[12] = cameraPos.x;
        fullMat[13] = cameraPos.y;
        fullMat[14] = cameraPos.z;
        fullMat[15] = 1.0f;

        if (ValidWeakPointer<Object3D>::isInitialized(this->parent)) {
            ValidWeakPointer<Object3D> parent(this->parent);
            parent->getTransform().updateWorldMatrix();
            Matrix4x4 parentMat = parent->getTransform().getWorldMatrix();
            parentMat.invert();
            full.preMultiply(parentMat);
        }

        this->transform.getLocalMatrix().copy(fullMat);
    }

    void Camera::project(Vector3Base<Real>& vec) {
        Core::Matrix4x4 projection = this->projectionMatrix;
        Real w = vec.getW();
        projection.transform(vec, w == 0.0f ? false : true);
        if (w != 0) {
            vec.x /= w;
            vec.y /= w;
            vec.z /= w;
        }
    }

    void Camera::unProject(Vector3Base<Real>& vec) {
        Core::Matrix4x4 projection = this->projectionMatrix;
        projection.invert();
        Real w = vec.getW();
        projection.transform(vec, w == 0.0f ? false : true);
        if (w != 0) {
            vec.x /= w;
            vec.y /= w;
            vec.z /= w;
        }
    }

    void Camera::buildPerspectiveProjectionMatrix(Real fov, Real ratio, Real nearP, Real farP, Matrix4x4& out) {
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
