//
// Created by Mark Kellogg on 12/18/17.
//

#include "Transform.h"

namespace MPM {

  Transform::Transform() {
    this->matrix.setIdentity();
  }

  Transform::Transform(const Matrix4x4& matrix) {
    this->copy(matrix);
  }

  Transform::~Transform() {

  }

  Matrix4x4& Transform::getMatrix() {
    return this->matrix;
  }

  const Matrix4x4& Transform::getConstMatrix() const {
    return this->matrix;
  }

  /*
   * Overloaded assignment operator
   */
  const Transform& Transform::operator =(const Transform& source) {
    if (this == &source) return *this;
    this->copy(source);
    return *this;
  }

  /*
   * Copy this Transform object's matrix into [dest].
   */
  void Transform::toMatrix(Matrix4x4& dest) const {
    dest.copy(this->matrix);
  }

  /*
   * Make this transform equal to [transform]
   */
  void Transform::copy(const Transform& transform) {
    this->copy(transform.matrix);
  }

  /*
	 * Copy [matrix] into the underlying Matrix4x4 that is encapsulated by this transform.
	 */
  void Transform::copy(const Matrix4x4& matrix) {
    this->matrix.copy(matrix);
  }

  /*
	 * Set this transform's matrix data to [matrixData].
	 */
  void Transform::copy(const Real * matrixData) {
    this->matrix.copy(matrixData);
  }

  void Transform::setIdentity() {
    this->matrix.setIdentity();
  }

  void Transform::invert() {
    this->matrix.invert();
  }

  /*
   * Post-multiply this transform's matrix by [transform]'s matrix.
   */
  void Transform::transformBy(const Transform& transform) {
    this->transformBy(transform.matrix);
  }

  /*
   * Pre-multiply this transform's matrix by [transform]'s matrix.
   */
  void Transform::preTransformBy(const Transform& transform) {
    this->preTransformBy(transform.matrix);
  }

  /*
   * Post-multiply this transform's matrix by [matrix]
   */
  void Transform::transformBy(const Matrix4x4& matrix) {
    this->matrix.multiply(matrix);
  }

  /*
   * Post-multiply this transform's matrix by [matrix].
   */
  void Transform::preTransformBy(const Matrix4x4& matrix) {
    this->matrix.preMultiply(matrix);
  }

}
