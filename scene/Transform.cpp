#include "Object3D.h"
#include "../util/WeakPointer.h"

namespace Core {

  Transform::Transform(const Object3D& target): target(target) {
    this->localMatrix.setIdentity();
    this->worldMatrix.setIdentity();
  }

  Transform::Transform(const Object3D& target, const Matrix4x4& matrix): target(target) {
    this->localMatrix.copy(matrix);
  }

  Transform::~Transform() {

  }

  Matrix4x4& Transform::getLocalMatrix() {
    return this->localMatrix;
  }

  Matrix4x4& Transform::getWorldMatrix() {
    return this->worldMatrix;
  }

  const Matrix4x4& Transform::getConstLocalMatrix() const {
    return this->localMatrix;
  }

  const Matrix4x4& Transform::getConstWorldMatrix() const {
    return this->worldMatrix;
  }

  /*
   * Copy this Transform object's local matrix into [dest].
   */
  void Transform::toLocalMatrix(Matrix4x4& dest) const {
    dest.copy(this->localMatrix);
  }

  /*
   * Copy this Transform object's world matrix into [dest].
   */
  void Transform::toWorldMatrix(Matrix4x4& dest) const {
    dest.copy(this->worldMatrix);
  }

  void Transform::updateWorldMatrix() {
    this->worldMatrix.copy(localMatrix);
    std::weak_ptr<Object3D> parent = const_cast<Object3D&>(target).getParent();
    while(WeakPointer<Object3D>::isValid(parent)) {
      WeakPointer<Object3D> parentPtr(parent);
      this->worldMatrix.preMultiply(parentPtr->getTransform().getLocalMatrix());
      parent = parentPtr->getParent();
    }
  }

  void Transform::transform(Vector4<Real>& vector) const {
    this->worldMatrix.transform(vector);
  }

  void Transform::transform(Vector3Base<Real>& vector, Bool asPoint) const {
    this->worldMatrix.transform(vector, asPoint);
  }

  void Transform::lookAt(const Point3r& target) {
        Point3r cameraPos;
        this->updateWorldMatrix();
        this->transform(cameraPos, true);

        Vector3r toTarget = target - cameraPos;
        toTarget.normalize();

        Vector3r vUp(0, 1, 0);
        Vector3r vRight;

        Vector3r::cross(toTarget, vUp, vRight);
        vRight.normalize();

        Vector3r::cross(vRight, toTarget, vUp);
        vUp.normalize();

        Matrix4x4 full = this->getLocalMatrix();
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

        std::weak_ptr<Object3D> parent = const_cast<Object3D&>(this->target).getParent();

        if (WeakPointer<Object3D>::isValid(parent)) {
            WeakPointer<Object3D> parent(parent);
            parent->getTransform().updateWorldMatrix();
            Matrix4x4 parentMat = parent->getTransform().getWorldMatrix();
            parentMat.invert();
            full.preMultiply(parentMat);
        }

        this->getLocalMatrix().copy(fullMat);
    }
}
