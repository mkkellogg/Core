#include "Object3D.h"
#include "../util/ValidWeakPointer.h"

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
    while(!parent.expired()) {
      ValidWeakPointer<Object3D> parentPtr(parent);
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
}
