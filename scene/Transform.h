//
// Created by Mark Kellogg on 12/18/17.
//

#ifndef MPM_TRANSFORM_H
#define MPM_TRANSFORM_H

#include  "../math/Matrix4x4.h"

namespace Core {

  class Transform {
    Matrix4x4 matrix;
  public:
    Transform();
    explicit Transform(const Matrix4x4& matrix);
    virtual ~Transform();

    const Transform& operator =(const Transform & source);

    Matrix4x4& getMatrix();
    const Matrix4x4& getConstMatrix() const;

    void toMatrix(Matrix4x4& dest) const;
    void copy(const Transform& transform);
    void copy(const Matrix4x4& matrix);
    void copy(const Real * matrixData);
    void setIdentity();
    void invert();
    void transformBy(const Transform& transform);
    void preTransformBy(const Transform& transform);
    void transformBy(const Matrix4x4& matrix);
    void preTransformBy(const Matrix4x4& matrix);
  };

}


#endif //MPM_TRANSFORM_H
