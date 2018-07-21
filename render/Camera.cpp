#include "string.h"

#include "Camera.h"
#include "../common/types.h"
#include "../math/Math.h"
#include "../math/Matrix4x4.h"
#include "../math/Quaternion.h"
#include "../util/WeakPointer.h"
#include "../scene/Object3D.h"

namespace Core {

    const UInt32 Camera::DEFAULT_FOV = 70;
    const UInt32 Camera::DEFAULT_WIDTH = 1200;
    const UInt32 Camera::DEFAULT_HEIGHT = 800;
    const Real Camera::DEFAULT_ASPECT_RATIO = (Real)Camera::DEFAULT_WIDTH / (Real)Camera::DEFAULT_HEIGHT;
    const Real Camera::DEFAULT_NEARP = 0.1;
    const Real Camera::DEFAULT_FARP = 100.0;

    Camera::Camera(WeakPointer<Object3D> owner): Object3DComponent(owner) {
    }

    void Camera::updateProjection() {
        if (this->ortho) {
            Camera::buildOrthographicProjectionMatrix(this->top, this->bottom, this->left, this->right, this->near, this->far, this->projectionMatrix);
        }
        else {
            Camera::buildPerspectiveProjectionMatrix(this->fov, this->aspectRatio, this->near, this->far, this->projectionMatrix);
        }
    }

    void Camera::setAspectRatio(Real ratio) {
        this->aspectRatio = ratio;
        this->updateProjection();
    }

    void Camera::setAspectRatioFromDimensions(UInt32 width, UInt32 height) {
        this->aspectRatio = (Real)width / (Real)height;
        this->updateProjection();
    }

    void Camera::setDimensions(Real top, Real  bottom, Real left, Real right) {
        this->top = top;
        this->bottom = bottom;
        this->left = left;
        this->right = right;
        this->updateProjection();
    }

    void Camera::setNear(Real near) {
        this->setNearAndFar(near, this->far);
    }

    void Camera::setFar(Real far) {
        this->setNearAndFar(this->near, far);
    }

    void Camera::setNearAndFar(Real near, Real far) {
        this->near = near;
        this->far = far;
        this->updateProjection();
    }

    Real Camera::getAspectRatio() const {
        return this->aspectRatio;
    }

    Real Camera::getFOV() const {
        return this->fov;
    }

    Real Camera::getNear() {
        return this->near;
    }

    Real Camera::getFar() {
        return this->far;
    }

    const Matrix4x4& Camera::getProjectionMatrix() const {
        return this->projectionMatrix;
    }

    void Camera::lookAt(const Point3r& target) {
        WeakPointer<Object3D> owner(this->getOwner());
        owner->getTransform().lookAt(target);
    }

    void Camera::project(Vector3Base<Real>& vec) const {
        Core::Matrix4x4 projection = this->projectionMatrix;
        Real w = vec.getW();
        projection.transform(vec);
        if (w != 0) {
            vec.x /= w;
            vec.y /= w;
            vec.z /= w;
        }
    }

    void Camera::unProject(Vector3Base<Real>& vec) const {
        Core::Matrix4x4 projection = this->projectionMatrix;
        projection.invert();
        Real w = vec.getW();
        projection.transform(vec);
        if (w != 0) {
            vec.x /= w;
            vec.y /= w;
            vec.z /= w;
        }
    }

    void Camera::setRenderTarget(WeakPointer<RenderTarget> renderTarget) {
        this->renderTarget = renderTarget;
    }

    WeakPointer<RenderTarget> Camera::getRenderTarget() {
        return this->renderTarget;
    }

    void Camera::setOrtho(Bool ortho) {
        this->ortho = ortho;
    }

    Bool Camera::isOrtho() const {
        return this->ortho;
    }

    void Camera::buildPerspectiveProjectionMatrix(Real fov, Real ratio, Real nearP, Real farP, Matrix4x4& out) {
        // convert fov to radians
        Real f = 1.0f / Math::tan(fov * .5f);

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

    void Camera::buildOrthographicProjectionMatrix(Real top, Real bottom, Real left, Real right, Real near, Real far, Matrix4x4& matrix) {
        matrix.setIdentity();

        Real data[16];
        memset(data, 0, 16 * sizeof(Real));

        Real r_width = 1.0f / (right - left);
        Real r_height = 1.0f / (top - bottom);
        Real r_depth = 1.0f / (far - near);
        Real x = 2.0f * (r_width);
        Real y = 2.0f * (r_height);
        Real z = -2.0f * (r_depth);
        Real tx = -(right + left) * r_width;
        Real ty = -(top + bottom) * r_height;
        Real tz = -(far + near) * r_depth;
        data[0] = x;
        data[5] = y;
        data[10] = z;
        data[12] = tx;
        data[13] = ty;
        data[14] = tz;
        data[15] = 1.0f;
        data[1] = 0.0f;
        data[2] = 0.0f;
        data[3] = 0.0f;
        data[4] = 0.0f;
        data[6] = 0.0f;
        data[7] = 0.0f;
        data[8] = 0.0f;
        data[9] = 0.0f;
        data[11] = 0.0f;

        matrix.copy(data);
    }

    Camera* Camera::createPerspectiveCamera(WeakPointer<Object3D> owner, Real fov, Real aspectRatio, Real near, Real far) {
        Camera * cam = new(std::nothrow) Camera(owner);
        if (cam == nullptr) {
            throw AllocationException("Camera::createPerspectiveCamera -> Could not allocation new Camera object.");
        }
        cam->fov = fov;
        cam->aspectRatio = aspectRatio;
        cam->near = near;
        cam->far = far;
        cam->ortho = false;
        Camera::buildPerspectiveProjectionMatrix(fov, aspectRatio, near, far, cam->projectionMatrix);
        return cam;
    }

    Camera* Camera::createOrthographicCamera(WeakPointer<Object3D> owner, Real top, Real bottom, Real left, Real right, Real near, Real far) {
        Camera * cam = new(std::nothrow) Camera(owner);
        if (cam == nullptr) {
            throw AllocationException("Camera::createOrthographicCamera -> Could not allocation new Camera object.");
        }
        cam->top = top;
        cam->bottom = bottom;
        cam->left = left;
        cam->right = right;
        cam->near = near;
        cam->far = far;
        cam->ortho = true;
        Camera::buildOrthographicProjectionMatrix(top, bottom, left, right, near, far, cam->projectionMatrix);
        return cam;
    }
}
