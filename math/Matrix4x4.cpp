#include <string.h>
#include "Matrix4x4.h"
#include "Quaternion.h"
#include "../common/debug.h"
#include "../common/assert.h"

namespace Core {

#define I(_i, _j) ((_j) + ROWSIZE_MATRIX_4X4 * (_i))

/*********************************************
*
* Matrix math utilities. These methods operate on OpenGL ES format matrices and
* vectors stored in Real arrays. Matrices are 4 x 4 column-vector matrices
* stored in column-major order:
*
* m[offset +  0] m[offset +  4] m[offset +  8] m[offset + 12]
* m[offset +  1] m[offset +  5] m[offset +  9] m[offset + 13]
* m[offset +  2] m[offset +  6] m[offset + 10] m[offset + 14]
* m[offset +  3] m[offset +  7] m[offset + 11] m[offset + 15]
*
* Vectors are 4 row x 1 column column-vectors stored in order:
* v[offset + 0]
* v[offset + 1]
* v[offset + 2]
* v[offset + 3]
*
***********************************************/

  Matrix4x4::Matrix4x4() {
    this->setIdentity();
  }

  Matrix4x4::Matrix4x4(const Real *sourceData) {
    if (sourceData != nullptr) {
      this->copy(sourceData);
    } else {
      this->setIdentity();
      Debug::PrintError("Matrix4x4::Matrix4x4(Real *) -> Null data passed.");
    }
  }

  Matrix4x4::Matrix4x4(const Matrix4x4 &source) {
    this->copy(source);
  }

  Matrix4x4::~Matrix4x4() {

  }

  Real *Matrix4x4::getData() {
    return this->data;
  }

  const Real *Matrix4x4::getConstData() const {
    return this->data;
  }

  void Matrix4x4::setIdentity() {
    setIdentity(this->data);
  }

  void Matrix4x4::setIdentity(Real *target) {
    ASSERT(target != nullptr, "Matrix4x4::setIdentity -> 'target' is null.");

    for (Int32 i = 0; i < SIZE_MATRIX_4X4; i++) {
      target[i] = 0;
    }

    for (Int32 i = 0; i < SIZE_MATRIX_4X4; i += 5) {
      target[i] = 1.0f;
    }
  }

  /*
	 * Overloaded assignment operator
	 */
  Matrix4x4 &Matrix4x4::operator=(const Matrix4x4 &source) {
    if (this == &source)return *this;
    this->copy(source);
    return *this;
  }

  /*
	 * Copy data from existing matrix to this one
	 */
  void Matrix4x4::copy(const Matrix4x4 &src) {
    if (this == &src)return;
    this->copy(src.data);
  }

  /*
   * Copy existing matrix data (from a Real array) to this one
   */
  void Matrix4x4::copy(const Real *sourceData) {
    ASSERT(sourceData != nullptr, "Matrix4x4::copy -> 'srcData' is null");
    memcpy(this->data, sourceData, sizeof(Real) * SIZE_MATRIX_4X4);
  }

  /*
	 * Transpose this matrix
	 */
  void Matrix4x4::transpose() {
    Real temp[SIZE_MATRIX_4X4];
    this->transpose(data, temp);
    memcpy(data, temp, sizeof(Real) * SIZE_MATRIX_4X4);
  }

  /*
   * Transpose the 4x4 matrix pointed to by [source] and store in [dest].
   */
  void Matrix4x4::transpose(const Real *source, Real *dest) {
    ASSERT(source != nullptr, "Matrix4x4::transpose -> 'source' is null.");
    ASSERT(dest != nullptr, "Matrix4x4::transpose -> 'dest' is null.");

    for (Int32 i = 0; i < ROWSIZE_MATRIX_4X4; i++) {
      Int32 mBase = i * ROWSIZE_MATRIX_4X4;
      dest[i] = source[mBase];
      dest[i + ROWSIZE_MATRIX_4X4] = source[mBase + 1];
      dest[i + ROWSIZE_MATRIX_4X4 * 2] = source[mBase + 2];
      dest[i + ROWSIZE_MATRIX_4X4 * 3] = source[mBase + 3];
    }
  }

  /*
  * Invert this matrix.
  *
  * Returns false if the matrix cannot be inverted
  */
  Bool Matrix4x4::invert() {
    Real temp[SIZE_MATRIX_4X4];
    Bool success = this->invert(this->data, temp);
    if (success == true) {
      memcpy(this->data, temp, sizeof(Real) * SIZE_MATRIX_4X4);
    }
    return success;
  }

  /*
   * Invert the 4x4 matrix pointed to by [out].
   *
   * Returns false if the matrix cannot be inverted
   */
  Bool Matrix4x4::invert(Matrix4x4 &out) {
    return invert(this->data, out.data);
  }

  /*
   * Invert the 4x4 matrix pointed to by [source] and store the result in [dest]
   *
   * Returns false if the matrix cannot be inverted
   */
  Bool Matrix4x4::invert(const Real *source, Real *dest) {
    // we need to know if the matrix is affine so that we can make it affine
    // once again after the inversion. the inversion process can introduce very small
    // precision errors that accumulate over time and eventually
    // result in a non-affine matrix
    Bool isAffine = Matrix4x4::isAffine(source);

    ASSERT(source != nullptr, "Matrix4x4::invert -> 'source' is null.");
    ASSERT(dest != nullptr, "Matrix4x4::invert -> 'dest' is null.");

    Real adjoin[SIZE_MATRIX_4X4];
    Real det = Matrix4x4::calculateDeterminant(source, adjoin);

    if (det == 0.0f) {
      return false;
    }

    // calculate matrix inverse
    det = 1 / det;
    for (Int32 j = 0; j < SIZE_MATRIX_4X4; j++)
      dest[j] = adjoin[j] * det;

    // if the matrix was affine before inversion, make it affine again
    // to avoid accumulating preicision errors
    if (isAffine) {
      dest[3] = 0;
      dest[7] = 0;
      dest[11] = 0;
      dest[15] = 1;
    }

    return true;
  }

  /*
 * Build matrix from components [translation], [scale], and [rotation]
 */
  void Matrix4x4::buildFromComponents(const Vector3Components<Real> &translation, const Quaternion &rotation,
                                      const Vector3Components<Real> &scale) {
    Matrix4x4 rotMatrix = rotation.rotationMatrix();

    // Build the final matrix, with translation, scale, and rotation
    A0 = scale.x * rotMatrix.A0;
    A1 = scale.y * rotMatrix.A1;
    A2 = scale.z * rotMatrix.A2;
    A3 = translation.x;
    B0 = scale.x * rotMatrix.B0;
    B1 = scale.y * rotMatrix.B1;
    B2 = scale.z * rotMatrix.B2;
    B3 = translation.y;
    C0 = scale.x * rotMatrix.C0;
    C1 = scale.y * rotMatrix.C1;
    C2 = scale.z * rotMatrix.C2;
    C3 = translation.z;

    D0 = 0;
    D1 = 0;
    D2 = 0;
    D3 = 1;
  }

  void
  Matrix4x4::decompose(Vector3Components<Real>& translation, Quaternion &rotation, Vector3Components<Real>& scale) const {
    ASSERT(isAffine(), "Matrix4x4::decompose -> Matrix is not affine.");

    Matrix4x4 rotMatrix;

    // build orthogonal matrix [rotMatrix]
    Real fInvLength = Math::InverseSquareRoot(A0 * A0 + B0 * B0 + C0 * C0);

    rotMatrix.A0 = A0 * fInvLength;
    rotMatrix.B0 = B0 * fInvLength;
    rotMatrix.C0 = C0 * fInvLength;

    Real fDot = rotMatrix.A0 * A1 + rotMatrix.B0 * B1 + rotMatrix.C0 * C1;
    rotMatrix.A1 = A1 - fDot * rotMatrix.A0;
    rotMatrix.B1 = B1 - fDot * rotMatrix.B0;
    rotMatrix.C1 = C1 - fDot * rotMatrix.C0;
    fInvLength = Math::InverseSquareRoot(
        rotMatrix.A1 * rotMatrix.A1 + rotMatrix.B1 * rotMatrix.B1 + rotMatrix.C1 * rotMatrix.C1);

    rotMatrix.A1 *= fInvLength;
    rotMatrix.B1 *= fInvLength;
    rotMatrix.C1 *= fInvLength;

    fDot = rotMatrix.A0 * A2 + rotMatrix.B0 * B2 + rotMatrix.C0 * C2;
    rotMatrix.A2 = A2 - fDot * rotMatrix.A0;
    rotMatrix.B2 = B2 - fDot * rotMatrix.B0;
    rotMatrix.C2 = C2 - fDot * rotMatrix.C0;

    fDot = rotMatrix.A1 * A2 + rotMatrix.B1 * B2 + rotMatrix.C1 * C2;
    rotMatrix.A2 -= fDot * rotMatrix.A1;
    rotMatrix.B2 -= fDot * rotMatrix.B1;
    rotMatrix.C2 -= fDot * rotMatrix.C1;

    fInvLength = Math::InverseSquareRoot(
        rotMatrix.A2 * rotMatrix.A2 + rotMatrix.B2 * rotMatrix.B2 + rotMatrix.C2 * rotMatrix.C2);

    rotMatrix.A2 *= fInvLength;
    rotMatrix.B2 *= fInvLength;
    rotMatrix.C2 *= fInvLength;

    // guarantee that orthogonal matrix has determinant 1 (no reflections)
    Real fDet =
        rotMatrix.A0 * rotMatrix.B1 * rotMatrix.C2 + rotMatrix.A1 * rotMatrix.B2 * rotMatrix.C0 +
        rotMatrix.A2 * rotMatrix.B0 * rotMatrix.C1 - rotMatrix.A2 * rotMatrix.B1 * rotMatrix.C0 -
        rotMatrix.A1 * rotMatrix.B0 * rotMatrix.C2 - rotMatrix.A0 * rotMatrix.B2 * rotMatrix.C1;

    if (fDet < 0.0) {
      for (size_t iRow = 0; iRow < 3; iRow++)
        for (size_t iCol = 0; iCol < 3; iCol++)
          rotMatrix.data[iCol * ROWSIZE_MATRIX_4X4 + iRow] = -rotMatrix.data[
              iCol * ROWSIZE_MATRIX_4X4 + iRow];
    }

    // build "right" matrix [rightMatrix]
    Matrix4x4 rightMatrix;
    rightMatrix.A0 = rotMatrix.A0 * A0 + rotMatrix.B0 * B0 + rotMatrix.C0 * C0;
    rightMatrix.A1 = rotMatrix.A0 * A1 + rotMatrix.B0 * B1 + rotMatrix.C0 * C1;
    rightMatrix.B1 = rotMatrix.A1 * A1 + rotMatrix.B1 * B1 + rotMatrix.C1 * C1;
    rightMatrix.A2 = rotMatrix.A0 * A2 + rotMatrix.B0 * B2 + rotMatrix.C0 * C2;
    rightMatrix.B2 = rotMatrix.A1 * A2 + rotMatrix.B1 * B2 + rotMatrix.C1 * C2;
    rightMatrix.C2 = rotMatrix.A2 * A2 + rotMatrix.B2 * B2 + rotMatrix.C2 * C2;

    // the scaling component
    scale.x = rightMatrix.A0;
    scale.y = rightMatrix.B1;
    scale.z = rightMatrix.C2;

    Vector3r shear;

    // the shear component
    Real fInvD0 = 1.0f / scale.x;
    shear.x = rightMatrix.A1 * fInvD0;
    shear.y = rightMatrix.A2 * fInvD0;
    shear.z = rightMatrix.B2 / scale.y;

    rotation.fromMatrix(rotMatrix);
    translation.set(A3, B3, C3);
  }

  Bool Matrix4x4::isAffine(void) const {
    return D0 == 0 && D1 == 0 && D2 == 0 && D3 == 1;
  }

  Bool Matrix4x4::isAffine(const Real *data) {
    return data[3] == 0 && data[7] == 0 && data[11] == 0 && data[15] == 1;
  }

  /*
	* Calculate the determinant of this matrix.
	*/
  Real Matrix4x4::calculateDeterminant(Real *adjoinOut) const {
    return calculateDeterminant(this->data, adjoinOut);
  }

  Real Matrix4x4::calculateDeterminant(const Real *source, Real *adjoinOut) {
    // array of transpose source matrix
    Real transpose[SIZE_MATRIX_4X4];

    // transpose matrix
    Matrix4x4::transpose(source, transpose);

    // temp array for pairs
    Real temp[SIZE_MATRIX_4X4];
    Real adjoin[SIZE_MATRIX_4X4];

    // calculate pairs for first 8 elements (cofactors)
    temp[0] = transpose[10] * transpose[15];
    temp[1] = transpose[11] * transpose[14];
    temp[2] = transpose[9] * transpose[15];
    temp[3] = transpose[11] * transpose[13];
    temp[4] = transpose[9] * transpose[14];
    temp[5] = transpose[10] * transpose[13];
    temp[6] = transpose[8] * transpose[15];
    temp[7] = transpose[11] * transpose[12];
    temp[8] = transpose[8] * transpose[14];
    temp[9] = transpose[10] * transpose[12];
    temp[10] = transpose[8] * transpose[13];
    temp[11] = transpose[9] * transpose[12];

    // Holds the destination matrix while we're building it up.
    Real *dst = adjoinOut ? adjoinOut : adjoin;

    // calculate first 8 elements (cofactors)
    dst[0] = temp[0] * transpose[5] + temp[3] * transpose[6] + temp[4] * transpose[7];
    dst[0] -= temp[1] * transpose[5] + temp[2] * transpose[6] + temp[5] * transpose[7];
    dst[1] = temp[1] * transpose[4] + temp[6] * transpose[6] + temp[9] * transpose[7];
    dst[1] -= temp[0] * transpose[4] + temp[7] * transpose[6] + temp[8] * transpose[7];
    dst[2] = temp[2] * transpose[4] + temp[7] * transpose[5] + temp[10] * transpose[7];
    dst[2] -= temp[3] * transpose[4] + temp[6] * transpose[5] + temp[11] * transpose[7];
    dst[3] = temp[5] * transpose[4] + temp[8] * transpose[5] + temp[11] * transpose[6];
    dst[3] -= temp[4] * transpose[4] + temp[9] * transpose[5] + temp[10] * transpose[6];
    dst[4] = temp[1] * transpose[1] + temp[2] * transpose[2] + temp[5] * transpose[3];
    dst[4] -= temp[0] * transpose[1] + temp[3] * transpose[2] + temp[4] * transpose[3];
    dst[5] = temp[0] * transpose[0] + temp[7] * transpose[2] + temp[8] * transpose[3];
    dst[5] -= temp[1] * transpose[0] + temp[6] * transpose[2] + temp[9] * transpose[3];
    dst[6] = temp[3] * transpose[0] + temp[6] * transpose[1] + temp[11] * transpose[3];
    dst[6] -= temp[2] * transpose[0] + temp[7] * transpose[1] + temp[10] * transpose[3];
    dst[7] = temp[4] * transpose[0] + temp[9] * transpose[1] + temp[10] * transpose[2];
    dst[7] -= temp[5] * transpose[0] + temp[8] * transpose[1] + temp[11] * transpose[2];

    // calculate pairs for second 8 elements (cofactors)
    temp[0] = transpose[2] * transpose[7];
    temp[1] = transpose[3] * transpose[6];
    temp[2] = transpose[1] * transpose[7];
    temp[3] = transpose[3] * transpose[5];
    temp[4] = transpose[1] * transpose[6];
    temp[5] = transpose[2] * transpose[5];
    temp[6] = transpose[0] * transpose[7];
    temp[7] = transpose[3] * transpose[4];
    temp[8] = transpose[0] * transpose[6];
    temp[9] = transpose[2] * transpose[4];
    temp[10] = transpose[0] * transpose[5];
    temp[11] = transpose[1] * transpose[4];

    // calculate second 8 elements (cofactors)
    dst[8] = temp[0] * transpose[13] + temp[3] * transpose[14] + temp[4] * transpose[15];
    dst[8] -= temp[1] * transpose[13] + temp[2] * transpose[14] + temp[5] * transpose[15];
    dst[9] = temp[1] * transpose[12] + temp[6] * transpose[14] + temp[9] * transpose[15];
    dst[9] -= temp[0] * transpose[12] + temp[7] * transpose[14] + temp[8] * transpose[15];
    dst[10] = temp[2] * transpose[12] + temp[7] * transpose[13] + temp[10] * transpose[15];
    dst[10] -= temp[3] * transpose[12] + temp[6] * transpose[13] + temp[11] * transpose[15];
    dst[11] = temp[5] * transpose[12] + temp[8] * transpose[13] + temp[11] * transpose[14];
    dst[11] -= temp[4] * transpose[12] + temp[9] * transpose[13] + temp[10] * transpose[14];
    dst[12] = temp[2] * transpose[10] + temp[5] * transpose[11] + temp[1] * transpose[9];
    dst[12] -= temp[4] * transpose[11] + temp[0] * transpose[9] + temp[3] * transpose[10];
    dst[13] = temp[8] * transpose[11] + temp[0] * transpose[8] + temp[7] * transpose[10];
    dst[13] -= temp[6] * transpose[10] + temp[9] * transpose[11] + temp[1] * transpose[8];
    dst[14] = temp[6] * transpose[9] + temp[11] * transpose[11] + temp[3] * transpose[8];
    dst[14] -= temp[10] * transpose[11] + temp[2] * transpose[8] + temp[7] * transpose[9];
    dst[15] = temp[10] * transpose[10] + temp[4] * transpose[8] + temp[9] * transpose[9];
    dst[15] -= temp[8] * transpose[9] + temp[11] * transpose[10] + temp[5] * transpose[8];

    // calculate determinant
    Real det = transpose[0] * dst[0] + transpose[1] * dst[1] + transpose[2] * dst[2] +
               transpose[3] * dst[3];

    return det;
  }

  /*
	 * Multiply this matrix by the scalar [scalar]
	 */
  void Matrix4x4::multiplyByScalar(Real scalar) {
    for (UInt32 i = 0; i < SIZE_MATRIX_4X4; i++) {
      this->data[i] *= scalar;
    }
  }

  /*
	 * Transform [vector] by this matrix, and store the result in [out]
	 */
  void Matrix4x4::transform(const Vector4<Real>& vector, Vector4<Real>& out) const {
    const Real * vectorData = vector.getConstData();
    Matrix4x4::multiplyMV(this->data, vectorData, out.getData());
  }

  /*
   * Transform [vector] by this matrix
   */
  void Matrix4x4::transform(Vector4<Real>& vector) const {
    Real temp[ROWSIZE_MATRIX_4X4];
    Matrix4x4::multiplyMV(this->data, vector.getData(), temp);
    memcpy(vector.getData(), temp, sizeof(Real) * ROWSIZE_MATRIX_4X4);
  }

  /*
   * Special case of vector transformation: transform a Vector3 (that's really
   * a 4-element homogeneous vector) and use 'isPoint' to determine if the
   * w element should be 1 or 0.
   *
   * Store the result in [out].
   */
  void Matrix4x4::transform(const Vector3Base<Real>& vector, Vector3Base<Real>& out, Bool asPoint) const {
    Vector4<Real> temp(vector.x, vector.y, vector.z, asPoint ? 1.0f : 0.0f);
    this->transform(temp);
    out.x = temp.x;
    out.y = temp.y;
    out.z = temp.z;
  }

  /*
   * Special case of vector transformation: transform a Vector3 (that's really
   * a 4-element homogeneous vector) and use [isPoint] to determine if the
   * w element should be 1 or 0.
   */
  void  Matrix4x4::transform(Vector3Base<Real>& vector, Bool asPoint) const {
    Vector4<Real> temp(vector.x, vector.y, vector.z, asPoint ? 1.0f : 0.0f);
    this->transform(temp);
    vector.x = temp.x;
    vector.y = temp.y;
    vector.z = temp.z;
  }

  /*
   * Transform [vector4f] by this matrix
   */
  void Matrix4x4::transform(Real * vector4f) const {
    ASSERT(vector4f != nullptr, "Matrix4x4::transform(Real *) -> 'vector4f' is null.");
    Real temp[ROWSIZE_MATRIX_4X4];
    Matrix4x4::multiplyMV(this->data, vector4f, temp);
    memcpy(vector4f, temp, sizeof(Real) * ROWSIZE_MATRIX_4X4);
  }

  /*
   * Add [matrix] to this matrix
   */
  void Matrix4x4::add(const Matrix4x4& matrix) {
    for (UInt32 i = 0; i < SIZE_MATRIX_4X4; i++) {
      data[i] += matrix.data[i];
    }
  }

  /*
   * Post-multiply this matrix by [matrix]
   */
  void Matrix4x4::multiply(const Matrix4x4& matrix) {
    Real temp[SIZE_MATRIX_4X4];
    Matrix4x4::multiplyMM(this->data, matrix.data, temp);
    memcpy(this->data, temp, sizeof(Real) * SIZE_MATRIX_4X4);
  }

  /*
   * Pre-multiply this matrix by [matrix]
   */
  void Matrix4x4::preMultiply(const Matrix4x4& matrix) {
    Real temp[SIZE_MATRIX_4X4];
    Matrix4x4::multiplyMM(matrix.data, this->data, temp);
    memcpy(this->data, temp, sizeof(Real) * SIZE_MATRIX_4X4);
  }

  /*
   * Post-multiply this matrix by [matrix], and store the result in [out]
   */
  void Matrix4x4::multiply(const Matrix4x4& matrix, Matrix4x4& out) const {
    Matrix4x4::multiplyMM(this->data, matrix.data, out.data);
  }

  /*
   * Post-multiply [lhs] by [rhs], and store the result in [out]
   */
  void Matrix4x4::multiply(const Matrix4x4& lhs, const Matrix4x4& rhs, Matrix4x4& out) {
    Matrix4x4::multiplyMM(lhs.data, rhs.data, out.data);
  }

  /*
   * Transform the vector pointed to by [rhsVec], by the matrix pointed to by [lhsMat],
   * and store the result in [out]
   */
  void Matrix4x4::multiplyMV(const Real *lhsMat, const Real *rhsVec, Real *out) {
    ASSERT(lhsMat != nullptr, "Matrix4x4::multiplyMV -> 'lhsMat' is null.");
    ASSERT(rhsVec != nullptr, "Matrix4x4::multiplyMV -> 'rhsVec' is null.");
    ASSERT(out != nullptr, "Matrix4x4::multiplyMV -> 'out' is null.");
    Matrix4x4::mx4transform(rhsVec[0], rhsVec[1], rhsVec[2], rhsVec[3], lhsMat, out);
  }

  /*
   * Transform the homogeneous point/vector specified by [x,y,z,w], by the matrix pointed to by [matrix],
   * and store the result in [pDest]
   */
  void Matrix4x4::mx4transform(Real x, Real y, Real z, Real w, const Real *matrix, Real *pDest) {
    ASSERT(matrix != nullptr, "Matrix4x4::mx4transform -> 'lhsMat' is null.");
    ASSERT(pDest != nullptr, "Matrix4x4::mx4transform -> 'pDest' is null.");

    pDest[0] = matrix[0 + ROWSIZE_MATRIX_4X4 * 0] * x + matrix[0 + ROWSIZE_MATRIX_4X4 * 1] * y +
               matrix[0 + ROWSIZE_MATRIX_4X4 * 2] * z + matrix[0 + ROWSIZE_MATRIX_4X4 * 3] * w;
    pDest[1] = matrix[1 + ROWSIZE_MATRIX_4X4 * 0] * x + matrix[1 + ROWSIZE_MATRIX_4X4 * 1] * y +
               matrix[1 + ROWSIZE_MATRIX_4X4 * 2] * z + matrix[1 + ROWSIZE_MATRIX_4X4 * 3] * w;
    pDest[2] = matrix[2 + ROWSIZE_MATRIX_4X4 * 0] * x + matrix[2 + ROWSIZE_MATRIX_4X4 * 1] * y +
               matrix[2 + ROWSIZE_MATRIX_4X4 * 2] * z + matrix[2 + ROWSIZE_MATRIX_4X4 * 3] * w;
    pDest[3] = matrix[3 + ROWSIZE_MATRIX_4X4 * 0] * x + matrix[3 + ROWSIZE_MATRIX_4X4 * 1] * y +
               matrix[3 + ROWSIZE_MATRIX_4X4 * 2] * z + matrix[3 + ROWSIZE_MATRIX_4X4 * 3] * w;
  }

  /*********************************************************
  *
  * Multiply two 4x4 matrices (in Real array form) together and store the result in a third 4x4 matrix.
  * In matrix notation: out = lhs x rhs. Due to the way matrix multiplication works,
  * the [out] matrix will have the same effect as first multiplying by the [rhs] matrix,
  * then multiplying by the [lhs] matrix.
  *
  * Parameters:
  * [out] The Real array that holds the result.
  * [lhs] The Real array that holds the left-hand-side 4x4 matrix.
  * [rhs] The Real array that holds the right-hand-side 4x4 matrix.
  *
  *********************************************************/
  void Matrix4x4::multiplyMM(const Real *lhs, const Real *rhs, Real *out) {
    for (Int32 i = 0; i < ROWSIZE_MATRIX_4X4; i++) {
      const Real rhs_i0 = rhs[I(i, 0)];
      Real ri0 = lhs[I(0, 0)] * rhs_i0;
      Real ri1 = lhs[I(0, 1)] * rhs_i0;
      Real ri2 = lhs[I(0, 2)] * rhs_i0;
      Real ri3 = lhs[I(0, 3)] * rhs_i0;
      for (Int32 j = 1; j < ROWSIZE_MATRIX_4X4; j++) {
        const Real rhs_ij = rhs[I(i, j)];
        ri0 += lhs[I(j, 0)] * rhs_ij;
        ri1 += lhs[I(j, 1)] * rhs_ij;
        ri2 += lhs[I(j, 2)] * rhs_ij;
        ri3 += lhs[I(j, 3)] * rhs_ij;
      }
      out[I(i, 0)] = ri0;
      out[I(i, 1)] = ri1;
      out[I(i, 2)] = ri2;
      out[I(i, 3)] = ri3;
    }
  }

  /*
	 * Translate this matrix by [vector]
	 *
	 * This performs a post-transformation, in that it is equivalent to post-multiplying
	 * this matrix by a translation matrix
	 */
  void Matrix4x4::translate(const Vector3Components<Real>& vector) {
    this->translate(vector.x, vector.y, vector.z);
  }

  /*
	 * Translate this matrix by [x], [y], [z]
	 *
	 * This performs a post-transformation, in that it is equivalent to post-multiplying
	 * this matrix by a translation matrix
	 */
  void Matrix4x4::translate(Real x, Real y, Real z) {
    Matrix4x4::translate(this->data, this->data, x, y, z);
  }

  /*
   * Translate this matrix by  [vector].
   *
   * This performs a pre-transformation, in that it is equivalent to pre-multiplying
   * this matrix by a translation matrix
   */
  void Matrix4x4::preTranslate(const Vector3Components<Real>& vector) {
    this->preTranslate(vector.x, vector.y, vector.z);
  }

  /*
   * Translate this matrix by [x], [y], [z]
   *
   * This performs a pre-transformation, in that it is equivalent to pre-multiplying
   * this matrix by a translation matrix
   */
  void Matrix4x4::preTranslate(Real x, Real y, Real z) {
    Matrix4x4::translate(this->data, this->data, x, y, z);
  }

  /*
   * Translate the 4x4 matrix pointed to by [src] by [vector], and store in [out]
   *
   * This performs a post-transformation, in that it is equivalent to post-multiplying
   * [source] by a translation matrix
   */
  void Matrix4x4::translate(const Matrix4x4& src, Matrix4x4& out, const Vector3Components<Real>& vector) {
    Matrix4x4::translate(src.data, out.data, vector.x, vector.y, vector.z);
  }

  /*
   * Translate the 4x4 matrix pointed to by [source] by [x], [y], and [z], and store in [out]
   *
   * This performs a post-transformation, in that it is equivalent to post-multiplying
   * [source] by a translation matrix
   */
  void Matrix4x4::translate(const Matrix4x4& src, Matrix4x4& out, Real x, Real y, Real z) {
    Matrix4x4::translate(src.data, out.data, x, y, z);
  }

  /*
   * Translate the 4x4 matrix pointed to by [source] by [x], [y], and [z], and store in [dest]
   *
   * This performs a post-transformation, in that it is equivalent to post-multiplying
   * [source] by a translation matrix
   */
  void Matrix4x4::translate(const Real * source, Real * dest, Real x, Real y, Real z) {
    ASSERT(source != nullptr, "Matrix4x4::Translate -> 'source' is null.");

    if (source != dest) {
      for (Int32 i = 0; i < 12; i++) {
        dest[i] = source[i];
      }
    }

    for (Int32 i = 0; i < ROWSIZE_MATRIX_4X4; i++) {
      Int32 tmi = i;
      Int32 mi = i;
      dest[12 + tmi] = source[mi] * x + source[4 + mi] * y + source[8 + mi] * z + source[12 + mi];
    }
  }

  /*
   * Translate the 4x4 matrix pointed to by [source] by [x], [y], and [z], and store in [dest]
   *
   * This performs a pre-transformation, in that it is equivalent to pre-multiplying
   * [source] by a translation matrix
   */
  void Matrix4x4::preTranslate(const Real * source, Real * dest, Real x, Real y, Real z) {
    Matrix4x4 trans;
    trans.setIdentity();

    Real *matrixData = trans.getData();
    matrixData[12] = x;
    matrixData[13] = y;
    matrixData[14] = z;

    Matrix4x4::multiplyMM(matrixData, source, dest);
  }

  /*
   * Rotate this matrix around the [vector] axis by [a] degrees
   *
   * This performs a post-transformation, in that it is equivalent to post-multiplying
   * this matrix by a rotation matrix
   */
  void Matrix4x4::rotate(const Vector3Components<Real>& vector, Real a) {
    Real temp[SIZE_MATRIX_4X4];
    Real r[SIZE_MATRIX_4X4];
    Matrix4x4::setRotate(r, vector.x, vector.y, vector.z, a);
    Matrix4x4::multiplyMM(this->data, r, temp);
    memcpy(this->data, temp, sizeof(Real) * SIZE_MATRIX_4X4);
  }

  /*
   * Rotate this matrix around the [x], [y], [z] axis by [a] degrees
   *
   * This performs a post-transformation, in that it is equivalent to post-multiplying
   * this matrix by a rotation matrix
   */
  void Matrix4x4::rotate(Real x, Real y, Real z, Real a) {
    Real temp[SIZE_MATRIX_4X4];
    Real r[SIZE_MATRIX_4X4];
    Matrix4x4::setRotate(r, x, y, z, a);
    Matrix4x4::multiplyMM(data, r, temp);
    memcpy(data, temp, sizeof(Real) * SIZE_MATRIX_4X4);
  }

  /*
   * Rotate this matrix around the [vector] axis by [a] degrees
   *
   * This performs a pre-transformation, in that it is equivalent to pre-multiplying
   * this matrix by a rotation matrix
   */
  void Matrix4x4::preRotate(const Vector3Components<Real>& vector, Real a) {
    Real temp[SIZE_MATRIX_4X4];
    Real r[SIZE_MATRIX_4X4];
    Matrix4x4::setRotate(r, vector.x, vector.y, vector.z, a);
    Matrix4x4::multiplyMM(r, data, temp);
    memcpy(data, temp, sizeof(Real) * SIZE_MATRIX_4X4);
  }

  /*
   * Rotate this matrix around the [x], [y], [z] axis by [a] degrees
   *
   * This performs a pre-transformation, in that it is equivalent to pre-multiplying
   * this matrix by a rotation matrix
   */
  void Matrix4x4::preRotate(Real x, Real y, Real z, Real a) {
    Real temp[SIZE_MATRIX_4X4];
    Real r[SIZE_MATRIX_4X4];
    Matrix4x4::setRotate(r, x, y, z, a);
    Matrix4x4::multiplyMM(r, data, temp);
    memcpy(data, temp, sizeof(Real) * SIZE_MATRIX_4X4);
  }

  /*
   * Set this matrix to be a rotation matrix, with Euler angles [x], [y], [z]
   */
  void Matrix4x4::setRotateEuler(Real x, Real y, Real z) {
    Matrix4x4::setRotateEuler(data, x, y, z);
  }

  /*
   * Set the 4x4 matrix [m] to be a rotation matrix, around axis [x], [y], [z] by [a] degrees
   */
  void Matrix4x4::setRotate(Matrix4x4& m, Real x, Real y, Real z, Real a) {
    Matrix4x4::setRotate(m.data, x, y, z, a);
  }

  /*
   * Set the 4x4 matrix [rm] to be a rotation matrix, around axis [x], [y], [z] by [a] degrees
   */
  void Matrix4x4::setRotate(Real * rm, Real x, Real y, Real z, Real a) {
    ASSERT(rm != nullptr, "Matrix4x4::setRotate -> 'rm' is null.");

    rm[3] = 0;
    rm[7] = 0;
    rm[11] = 0;
    rm[12] = 0;
    rm[13] = 0;
    rm[14] = 0;
    rm[15] = 1;
    a *= Math::DegreesToRads;
    Real s = (Real)Math::Sin(a);
    Real c = (Real)Math::Cos(a);
    if (1.0f == x && 0.0f == y && 0.0f == z) {
      rm[5] = c;   rm[10] = c;
      rm[6] = s;   rm[9] = -s;
      rm[1] = 0;   rm[2] = 0;
      rm[4] = 0;   rm[8] = 0;
      rm[0] = 1;
    }
    else if (0.0f == x && 1.0f == y && 0.0f == z) {
      rm[0] = c;   rm[10] = c;
      rm[8] = s;   rm[2] = -s;
      rm[1] = 0;   rm[4] = 0;
      rm[6] = 0;   rm[9] = 0;
      rm[5] = 1;
    }
    else if (0.0f == x && 0.0f == y && 1.0f == z) {
      rm[0] = c;   rm[5] = c;
      rm[1] = s;   rm[4] = -s;
      rm[2] = 0;   rm[6] = 0;
      rm[8] = 0;   rm[9] = 0;
      rm[10] = 1;
    }
    else {
      Real len = Vector3r::magnitude(x, y, z);
      if (1.0f != len)
      {
        Real recipLen = 1.0f / len;
        x *= recipLen;
        y *= recipLen;
        z *= recipLen;
      }
      Real nc = 1.0f - c;
      Real xy = x * y;
      Real yz = y * z;
      Real zx = z * x;
      Real xs = x * s;
      Real ys = y * s;
      Real zs = z * s;
      rm[0] = x*x*nc + c;
      rm[4] = xy*nc - zs;
      rm[8] = zx*nc + ys;
      rm[1] = xy*nc + zs;
      rm[5] = y*y*nc + c;
      rm[9] = yz*nc - xs;
      rm[2] = zx*nc - ys;
      rm[6] = yz*nc + xs;
      rm[10] = z*z*nc + c;
    }
  }

  /*
   * Set the matrix [rm] to be a rotation matrix, with Euler angles [x], [y], [z]
   */
  void Matrix4x4::setRotateEuler(Real * rm, Real x, Real y, Real z) {
    ASSERT(rm != nullptr, "Matrix4x4::setRotateEuler -> 'rm' is null.");

    x *= Math::DegreesToRads;
    y *= Math::DegreesToRads;
    z *= Math::DegreesToRads;
    Real cx = (Real)Math::Cos(x);
    Real sx = (Real)Math::Sin(x);
    Real cy = (Real)Math::Cos(y);
    Real sy = (Real)Math::Sin(y);
    Real cz = (Real)Math::Cos(z);
    Real sz = (Real)Math::Sin(z);
    Real cxsy = cx * sy;
    Real sxsy = sx * sy;

    rm[0] = cy * cz;
    rm[1] = -cy * sz;
    rm[2] = sy;
    rm[3] = 0.0f;

    rm[4] = cxsy * cz + cx * sz;
    rm[5] = -cxsy * sz + cx * cz;
    rm[6] = -sx * cy;
    rm[7] = 0.0f;

    rm[8] = -sxsy * cz + sx * sz;
    rm[9] = sxsy * sz + sx * cz;
    rm[10] = cx * cy;
    rm[11] = 0.0f;

    rm[12] = 0.0f;
    rm[13] = 0.0f;
    rm[14] = 0.0f;
    rm[15] = 1.0f;
  }

  /*
	 * Scale this matrix by the x,y, and z components of [scale]
	 *
	 * This performs a post-transformation, in that it is equivalent to post-multiplying this
	 * matrix by a scale matrix
	 */
  void Matrix4x4::scale(const Vector3Components<Real>& scale) {
    this->scale(scale.x, scale.y, scale.z);
  }

  /*
   * Scale this matrix by [x], [y], and [z].
   *
   * This performs a post-transformation, in that it is equivalent to post-multiplying this
   * matrix by a scale matrix
   */
  void Matrix4x4::scale(Real x, Real y, Real z) {
    Real temp[SIZE_MATRIX_4X4];
    Matrix4x4::scale(this->data, temp, x, y, z);
    memcpy(data, temp, sizeof(Real) * SIZE_MATRIX_4X4);
  }

  /*
   * Scale this matrix by [x], [y], and [z].
   *
   * This performs a pre-transformation, in that it is equivalent to pre-multiplying this
   * matrix by a scale matrix
   */
  void Matrix4x4::preScale(Real x, Real y, Real z) {
    Real temp[SIZE_MATRIX_4X4];
    Matrix4x4::preScale(this->data, temp, x, y, z);
    memcpy(data, temp, sizeof(Real) * SIZE_MATRIX_4X4);
  }

  /*
   * Scale this matrix by [x], [y], and [z] and store the result in [out]
   *
   * This performs a post-transformation, in that it is equivalent to post-multiplying this
   * matrix by a scale matrix
   */
  void Matrix4x4::scale(Matrix4x4& out, Real x, Real y, Real z) const {
    Matrix4x4::scale(this->data, out.data, x, y, z);
  }

  /*
   * Scale the matrix pointed to by [source] by [x], [y], and [z], and store
   * the result in [dest]
   *
   * This performs a post-transformation, in that it is equivalent to post-multiplying
   * [source] by a scale matrix
   */
  void Matrix4x4::scale(const Real * source, Real * dest, Real x, Real y, Real z) {
    ASSERT(source != nullptr, "Matrix4x4::scale -> 'source' is null.");
    ASSERT(dest != nullptr, "Matrix4x4::scale -> 'dest' is null.");

    for (Int32 i = 0; i < ROWSIZE_MATRIX_4X4; i++) {
      Int32 smi = i;
      Int32 mi = i;
      dest[smi] = source[mi] * x;
      dest[4 + smi] = source[4 + mi] * y;
      dest[8 + smi] = source[8 + mi] * z;
      dest[12 + smi] = source[12 + mi];
    }
  }

  /*
   * Scale the matrix pointed to by [source] by [x], [y], and [z], and store
   * the result in [dest]
   *
   * This performs a pre-transformation, in that it is equivalent to pre-multiplying
   * [source] by a scale matrix
   */
  void Matrix4x4::preScale(const Real * source, Real * dest, Real x, Real y, Real z) {
    ASSERT(source != nullptr, "Matrix4x4::preScale -> 'source' is null.");
    ASSERT(dest != nullptr, "Matrix4x4::preScale -> 'dest' is null.");

    for (Int32 i = 0; i < ROWSIZE_MATRIX_4X4; i++) {
      Int32 smi = i;
      Int32 mi = i * 4;
      dest[smi] = source[mi] * x;
      dest[4 + smi] = source[1 + mi] * y;
      dest[8 + smi] = source[2 + mi] * z;
      dest[12 + smi] = source[3 + mi];
    }
  }
}
