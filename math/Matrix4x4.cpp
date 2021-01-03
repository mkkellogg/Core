#include "Matrix4x4.h"
#include <string.h>
#include "../common/Exception.h"
#include "../common/debug.h"
#include "Quaternion.h"

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

    Vector3r Matrix4x4::zero(0.0f, 0.0f, 0.0f);
    Vector3r Matrix4x4::one(1.0f, 1.0f, 1.0f);

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
        if (target == nullptr) throw NullPointerException("Matrix4x4::setIdentity -> 'target' is null.");

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
        if (this == &source) return *this;
        this->copy(source);
        return *this;
    }

    /*
     * Copy data from existing matrix to this one
     */
    void Matrix4x4::copy(const Matrix4x4 &src) {
        if (this == &src) return;
        this->copy(src.data);
    }

    /*
     * Copy existing matrix data (from a Real array) to this one
     */
    void Matrix4x4::copy(const Real *sourceData) {
        if (sourceData == nullptr) throw NullPointerException("Matrix4x4::copy -> 'srcData' is null");
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
        if (source == nullptr) throw NullPointerException("Matrix4x4::transpose -> 'source' is null.");
        if (dest == nullptr) throw NullPointerException("Matrix4x4::transpose -> 'dest' is null.");

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

        if (source == nullptr) throw NullPointerException("Matrix4x4::invert -> 'source' is null.");
        if (dest == nullptr) throw NullPointerException("Matrix4x4::invert -> 'dest' is null.");

        Real adjoin[SIZE_MATRIX_4X4];
        Real det = Matrix4x4::calculateDeterminant(source, adjoin);

        if (det == 0.0f) {
            return false;
        }

        // calculate matrix inverse
        det = 1 / det;
        for (Int32 j = 0; j < SIZE_MATRIX_4X4; j++) dest[j] = adjoin[j] * det;

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
    void Matrix4x4::compose(const Vector3Components<Real> &translation, const Quaternion &rotation, const Vector3Components<Real> &scale) {
        Real qx = rotation.x();
        Real qy = rotation.y();
        Real qz = rotation.z();
        Real qw = rotation.w();

		Real x2 = qx + qx;
        Real y2 = qy + qy;
        Real z2 = qz + qz;
		Real xx = qx * x2;
        Real xy = qx * y2;
        Real xz = qx * z2;
		Real yy = qy * y2;
        Real yz = qy * z2;
        Real zz = qz * z2;
		Real wx = qw * x2;
        Real wy = qw * y2;
        Real wz = qw * z2;

		Real sx = scale.x;
        Real sy = scale.y;
        Real sz = scale.z;

		this->A0 = (1.0f - (yy + zz)) * sx;
		this->B0 = (xy + wz) * sx;
		this->C0 = (xz - wy) * sx;
		this->D0 = 0.0f;

		this->A1 = (xy - wz) * sy;
		this->B1 = (1.0f - (xx + zz)) * sy;
		this->C1 = (yz + wx) * sy;
		this->D1 = 0.0f;

		this->A2 = (xz + wy) * sz;
		this->B2 = (yz - wx) * sz;
		this->C2 = (1.0f - (xx + yy)) * sz;
		this->D2 = 0.0f;

		this->A3 = translation.x;
		this->B3 = translation.y;
		this->C3 = translation.z;
		this->D3 = 1.0f;
    }

    void Matrix4x4::compose(const Vector3Components<Real>& translation, const Vector3Components<Real>& euler, const Vector3Components<Real>& scale) {
        Matrix4x4 temp;
        temp.makeRotationFromEuler(euler);
        this->makeScale(scale);
        this->preMultiply(temp);
        this->preTranslate(translation);
    }

    void Matrix4x4::decompose(Vector3Components<Real> &translation, Quaternion &rotation, Vector3Components<Real> &scale) const {
        if (!isAffine()) throw NullPointerException("Matrix4x4::decompose -> Matrix is not affine.");

        Matrix4x4 rotMatrix;

        // build orthogonal matrix [rotMatrix]
        Real fInvLength = Math::inverseSquareRoot(A0 * A0 + B0 * B0 + C0 * C0);

        rotMatrix.A0 = A0 * fInvLength;
        rotMatrix.B0 = B0 * fInvLength;
        rotMatrix.C0 = C0 * fInvLength;

        Real fDot = rotMatrix.A0 * A1 + rotMatrix.B0 * B1 + rotMatrix.C0 * C1;
        rotMatrix.A1 = A1 - fDot * rotMatrix.A0;
        rotMatrix.B1 = B1 - fDot * rotMatrix.B0;
        rotMatrix.C1 = C1 - fDot * rotMatrix.C0;
        fInvLength = Math::inverseSquareRoot(rotMatrix.A1 * rotMatrix.A1 + rotMatrix.B1 * rotMatrix.B1 + rotMatrix.C1 * rotMatrix.C1);

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

        fInvLength = Math::inverseSquareRoot(rotMatrix.A2 * rotMatrix.A2 + rotMatrix.B2 * rotMatrix.B2 + rotMatrix.C2 * rotMatrix.C2);

        rotMatrix.A2 *= fInvLength;
        rotMatrix.B2 *= fInvLength;
        rotMatrix.C2 *= fInvLength;

        // guarantee that orthogonal matrix has determinant 1 (no reflections)
        Real fDet = rotMatrix.A0 * rotMatrix.B1 * rotMatrix.C2 + rotMatrix.A1 * rotMatrix.B2 * rotMatrix.C0 + rotMatrix.A2 * rotMatrix.B0 * rotMatrix.C1 -
                    rotMatrix.A2 * rotMatrix.B1 * rotMatrix.C0 - rotMatrix.A1 * rotMatrix.B0 * rotMatrix.C2 - rotMatrix.A0 * rotMatrix.B2 * rotMatrix.C1;

        if (fDet < 0.0) {
            for (size_t iRow = 0; iRow < 3; iRow++)
                for (size_t iCol = 0; iCol < 3; iCol++) rotMatrix.data[iCol * ROWSIZE_MATRIX_4X4 + iRow] = -rotMatrix.data[iCol * ROWSIZE_MATRIX_4X4 + iRow];
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
        Real det = transpose[0] * dst[0] + transpose[1] * dst[1] + transpose[2] * dst[2] + transpose[3] * dst[3];

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
    void Matrix4x4::transform(const Vector4<Real> &vector, Vector4<Real> &out) const {
        const Real *vectorData = const_cast<Vector4<Real>&>(vector).getData();
        Matrix4x4::multiplyMV(this->data, vectorData, out.getData());
    }

    /*
     * Transform [vector] by this matrix
     */
    void Matrix4x4::transform(Vector4<Real> &vector) const {
        Real temp[ROWSIZE_MATRIX_4X4];
        Matrix4x4::multiplyMV(this->data, vector.getData(), temp);
        memcpy(vector.getData(), temp, sizeof(Real) * ROWSIZE_MATRIX_4X4);
    }

    /*
     * Transform [vector] by this matrix. If the resulting [w] is not 1.0 or 0.0, the vector
     * will be scale by 1/[w] so that [w] = 1.0. This is ONLY done for a 3-component vector.
     *
     * Store the result in [out].
     */
    void Matrix4x4::transform(const Vector3Base<Real> &vector, Vector3Base<Real> &out) const {
        Real w = vector.getW();
        Vector4<Real> temp(vector.x, vector.y, vector.z, w);
        this->transform(temp);
        out.x = temp.x;
        out.y = temp.y;
        out.z = temp.z;
        if (temp.w != 1.0f && temp.w != 0.0f) {
            out.x /= temp.w;
            out.y /= temp.w;
            out.z /= temp.w;
        }
    }

    /*
     * Transform [vector] by this matrix
     */
    void Matrix4x4::transform(Vector3Base<Real> &vector) const {
        this->transform(vector, vector);
    }

    /*
     * Transform [vector4f] by this matrix
     */
    void Matrix4x4::transform(Real *vector4f) const {
        if (vector4f == nullptr) throw NullPointerException("Matrix4x4::transform(Real *) -> 'vector4f' is null.");
        Real temp[ROWSIZE_MATRIX_4X4];
        Matrix4x4::multiplyMV(this->data, vector4f, temp);
        memcpy(vector4f, temp, sizeof(Real) * ROWSIZE_MATRIX_4X4);
    }

    /*
     * Add [matrix] to this matrix
     */
    void Matrix4x4::add(const Matrix4x4 &matrix) {
        for (UInt32 i = 0; i < SIZE_MATRIX_4X4; i++) {
            data[i] += matrix.data[i];
        }
    }

    /*
     * Post-multiply this matrix by [matrix]
     */
    void Matrix4x4::multiply(const Matrix4x4 &matrix) {
        Real temp[SIZE_MATRIX_4X4];
        Matrix4x4::multiplyMM(this->data, matrix.data, temp);
        memcpy(this->data, temp, sizeof(Real) * SIZE_MATRIX_4X4);
    }

    /*
     * Pre-multiply this matrix by [matrix]
     */
    void Matrix4x4::preMultiply(const Matrix4x4 &matrix) {
        Real temp[SIZE_MATRIX_4X4];
        Matrix4x4::multiplyMM(matrix.data, this->data, temp);
        memcpy(this->data, temp, sizeof(Real) * SIZE_MATRIX_4X4);
    }

    /*
     * Post-multiply this matrix by [matrix], and store the result in [out]
     */
    void Matrix4x4::multiply(const Matrix4x4 &matrix, Matrix4x4 &out) const {
        Matrix4x4::multiplyMM(this->data, matrix.data, out.data);
    }

    /*
     * Post-multiply [lhs] by [rhs], and store the result in [out]
     */
    void Matrix4x4::multiply(const Matrix4x4 &lhs, const Matrix4x4 &rhs, Matrix4x4 &out) {
        Matrix4x4::multiplyMM(lhs.data, rhs.data, out.data);
    }

    /*
     * Transform the vector pointed to by [rhsVec], by the matrix pointed to by [lhsMat],
     * and store the result in [out]
     */
    void Matrix4x4::multiplyMV(const Real *lhsMat, const Real *rhsVec, Real *out) {
        if (lhsMat == nullptr) throw NullPointerException("Matrix4x4::multiplyMV -> 'lhsMat' is null.");
        if (rhsVec == nullptr) throw NullPointerException("Matrix4x4::multiplyMV -> 'rhsVec' is null.");
        if (out == nullptr) throw NullPointerException("Matrix4x4::multiplyMV -> 'out' is null.");
        Matrix4x4::mx4transform(rhsVec[0], rhsVec[1], rhsVec[2], rhsVec[3], lhsMat, out);
    }

    /*
     * Transform the homogeneous point/vector specified by [x,y,z,w], by the matrix pointed to by [matrix],
     * and store the result in [pDest]
     */
    void Matrix4x4::mx4transform(Real x, Real y, Real z, Real w, const Real *matrix, Real *pDest) {
        if (matrix == nullptr) throw NullPointerException("Matrix4x4::mx4transform -> 'lhsMat' is null.");
        if (pDest == nullptr) throw NullPointerException("Matrix4x4::mx4transform -> 'pDest' is null.");

        pDest[0] = matrix[0 + ROWSIZE_MATRIX_4X4 * 0] * x + matrix[0 + ROWSIZE_MATRIX_4X4 * 1] * y + matrix[0 + ROWSIZE_MATRIX_4X4 * 2] * z +
                   matrix[0 + ROWSIZE_MATRIX_4X4 * 3] * w;
        pDest[1] = matrix[1 + ROWSIZE_MATRIX_4X4 * 0] * x + matrix[1 + ROWSIZE_MATRIX_4X4 * 1] * y + matrix[1 + ROWSIZE_MATRIX_4X4 * 2] * z +
                   matrix[1 + ROWSIZE_MATRIX_4X4 * 3] * w;
        pDest[2] = matrix[2 + ROWSIZE_MATRIX_4X4 * 0] * x + matrix[2 + ROWSIZE_MATRIX_4X4 * 1] * y + matrix[2 + ROWSIZE_MATRIX_4X4 * 2] * z +
                   matrix[2 + ROWSIZE_MATRIX_4X4 * 3] * w;
        pDest[3] = matrix[3 + ROWSIZE_MATRIX_4X4 * 0] * x + matrix[3 + ROWSIZE_MATRIX_4X4 * 1] * y + matrix[3 + ROWSIZE_MATRIX_4X4 * 2] * z +
                   matrix[3 + ROWSIZE_MATRIX_4X4 * 3] * w;
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
    void Matrix4x4::translate(const Vector3Components<Real> &vector) {
        this->translate(vector.x, vector.y, vector.z);
    }

    /*
     * Translate this matrix by [x], [y], [z]
     *
     * This performs a post-transformation, in that it is equivalent to post-multiplying
     * this matrix by a translation matrix
     */
    void Matrix4x4::translate(Real x, Real y, Real z) {
        for (Int32 i = 0; i < ROWSIZE_MATRIX_4X4; i++) {
            Int32 tmi = i;
            Int32 mi = i;
            this->data[12 + tmi] = this->data[mi] * x + this->data[4 + mi] * y + this->data[8 + mi] * z + this->data[12 + mi];
        }
    }

    /*
     * Translate this matrix by  [vector].
     *
     * This performs a pre-transformation, in that it is equivalent to pre-multiplying
     * this matrix by a translation matrix
     */
    void Matrix4x4::preTranslate(const Vector3Components<Real> &vector) {
        this->preTranslate(vector.x, vector.y, vector.z);
    }

    /*
     * Translate this matrix by [x], [y], [z]
     *
     * This performs a pre-transformation, in that it is equivalent to pre-multiplying
     * this matrix by a translation matrix
     */
    void Matrix4x4::preTranslate(Real x, Real y, Real z) {
        this->data[12] = this->data[12] + x;
        this->data[13] = this->data[13] + y;
        this->data[14] = this->data[14] + z;
    }

    void Matrix4x4::setTranslation(Real x, Real y, Real z) {
        this->data[12] = x;
        this->data[13] = y;
        this->data[14] = z;
    }

    Vector3r Matrix4x4::getTranslation() {
        Vector3r translation(this->data[12], this->data[13], this->data[14]);
        return translation;
    }

    /*
     * Rotate this matrix around the [axis] by [a] radians.
     *
     * This performs a post-transformation, in that it is equivalent to post-multiplying
     * this matrix by a rotation matrix
     */
    void Matrix4x4::rotate(const Vector3Components<Real> &axis, Real a) {
        this->rotate(axis.x, axis.y, axis.z, a);
    }

    /*
     * Rotate this matrix around the [x], [y], [z] axis by [a] radians.
     *
     * This performs a post-transformation, in that it is equivalent to post-multiplying
     * this matrix by a rotation matrix
     */
    void Matrix4x4::rotate(Real x, Real y, Real z, Real a) {
        static Matrix4x4 r;
        r.makeRotation(x, y, z, a);
        this->multiply(r);
    }

    /*
     * Rotate this matrix around the [axis] by [a] radians.
     *
     * This performs a pre-transformation, in that it is equivalent to pre-multiplying
     * this matrix by a rotation matrix
     */
    void Matrix4x4::preRotate(const Vector3Components<Real> &axis, Real a) {
        this->preRotate(axis.x, axis.y, axis.z, a);
    }

    /*
     * Rotate this matrix around the [x], [y], [z] axis by [a] radians.
     *
     * This performs a pre-transformation, in that it is equivalent to pre-multiplying
     * this matrix by a rotation matrix
     */
    void Matrix4x4::preRotate(Real x, Real y, Real z, Real a) {
        static Matrix4x4 r;
        r.makeRotation(x, y, z, a);
        this->preMultiply(r);
    }

    /*
     * Set this to be a rotation matrix, around axis [axis] by [a] radians.
     */
    void Matrix4x4::makeRotation(const Vector3Components<Real> &axis, Real a) {
        this->makeRotation(axis.x, axis.y, axis.z, a);
    }

    /*
     * Set this to be a rotation matrix, around axis [x], [y], [z] by [a] radians.
     */
    void Matrix4x4::makeRotation(Real x, Real y, Real z, Real a) {
        Real *rm = this->data;
        rm[3] = 0;
        rm[7] = 0;
        rm[11] = 0;
        rm[12] = 0;
        rm[13] = 0;
        rm[14] = 0;
        rm[15] = 1;
        Real s = (Real)Math::sin(a);
        Real c = (Real)Math::cos(a);
        if (1.0f == x && 0.0f == y && 0.0f == z) {
            rm[5] = c;
            rm[10] = c;
            rm[6] = s;
            rm[9] = -s;
            rm[1] = 0;
            rm[2] = 0;
            rm[4] = 0;
            rm[8] = 0;
            rm[0] = 1;
        } else if (0.0f == x && 1.0f == y && 0.0f == z) {
            rm[0] = c;
            rm[10] = c;
            rm[8] = s;
            rm[2] = -s;
            rm[1] = 0;
            rm[4] = 0;
            rm[6] = 0;
            rm[9] = 0;
            rm[5] = 1;
        } else if (0.0f == x && 0.0f == y && 1.0f == z) {
            rm[0] = c;
            rm[5] = c;
            rm[1] = s;
            rm[4] = -s;
            rm[2] = 0;
            rm[6] = 0;
            rm[8] = 0;
            rm[9] = 0;
            rm[10] = 1;
        } else {
            Real len = Vector3r::magnitude(x, y, z);
            if (1.0f != len) {
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
            rm[0] = x * x * nc + c;
            rm[4] = xy * nc - zs;
            rm[8] = zx * nc + ys;
            rm[1] = xy * nc + zs;
            rm[5] = y * y * nc + c;
            rm[9] = yz * nc - xs;
            rm[2] = zx * nc - ys;
            rm[6] = yz * nc + xs;
            rm[10] = z * z * nc + c;
        }
    }

    /*
     * Set this matrix to be a rotation matrix, with Euler angles from the components of [eulers]
     */
    void Matrix4x4::makeRotationFromEuler(const Vector3Components<Real>& eulers) {
        this->makeRotationFromEuler(eulers.x, eulers.y, eulers.z);
    }

    /*
     * Set this matrix to be a rotation matrix, with Euler angles [x], [y], [z]
     */
    void Matrix4x4::makeRotationFromEuler(Real x, Real y, Real z) {
        Real * rm = this->data;

        const Real a = Math::cos(x), b = Math::sin(x);
		const Real c = Math::cos(y), d = Math::sin(y);
		const Real e = Math::cos(z), f = Math::sin(z);

        // Ordering: XYZ
        const Real ae = a * e, af = a * f, be = b * e, bf = b * f;

        rm[ 0 ] = c * e;
        rm[ 4 ] = - c * f;
        rm[ 8 ] = d;

        rm[ 1 ] = af + be * d;
        rm[ 5 ] = ae - bf * d;
        rm[ 9 ] = - b * c;

        rm[ 2 ] = bf - ae * d;
        rm[ 6 ] = be + af * d;
        rm[ 10 ] = a * c;
        

        // Ordering: ZYX
       /* const Real ae = a * e, af = a * f, be = b * e, bf = b * f;

        rm[ 0 ] = c * e;
        rm[ 4 ] = be * d - af;
        rm[ 8 ] = ae * d + bf;

        rm[ 1 ] = c * f;
        rm[ 5 ] = bf * d + ae;
        rm[ 9 ] = af * d - be;

        rm[ 2 ] = - d;
        rm[ 6 ] = b * c;
        rm[ 10 ] = a * c;*/

        // bottom row
		rm[ 3 ] = 0;
		rm[ 7 ] = 0;
		rm[ 11 ] = 0;

		// last column
		rm[ 12 ] = 0;
		rm[ 13 ] = 0;
		rm[ 14 ] = 0;
		rm[ 15 ] = 1;
    }

    /*
     * Scale this matrix by the x,y, and z components of [scale]
     *
     * This performs a post-transformation, in that it is equivalent to post-multiplying this
     * matrix by a scale matrix
     */
    void Matrix4x4::scale(const Vector3Components<Real> &scale) {
        this->scale(scale.x, scale.y, scale.z);
    }

    /*
     * Scale this matrix by [x], [y], and [z].
     *
     * This performs a post-transformation, in that it is equivalent to post-multiplying this
     * matrix by a scale matrix
     */
    void Matrix4x4::scale(Real x, Real y, Real z) {
        for (Int32 i = 0; i < ROWSIZE_MATRIX_4X4; i++) {
            Int32 smi = i;
            Int32 mi = i;
            this->data[smi] = this->data[mi] * x;
            this->data[4 + smi] = this->data[4 + mi] * y;
            this->data[8 + smi] = this->data[8 + mi] * z;
            this->data[12 + smi] = this->data[12 + mi];
        }
    }

    /*
     * Scale this matrix by the components of [scale].
     *
     * This performs a pre-transformation, in that it is equivalent to pre-multiplying this
     * matrix by a scale matrix
     */
    void Matrix4x4::preScale(const Vector3Components<Real>& scale) {
        this->preScale(scale.x, scale.y, scale.z);
    }

    /*
     * Scale this matrix by [x], [y], and [z].
     *
     * This performs a pre-transformation, in that it is equivalent to pre-multiplying this
     * matrix by a scale matrix
     */
    void Matrix4x4::preScale(Real x, Real y, Real z) {
        for (Int32 i = 0; i < ROWSIZE_MATRIX_4X4; i++) {
            Int32 smi = i;
            Int32 mi = i * 4;
            this->data[smi] = this->data[mi] * x;
            this->data[4 + smi] = this->data[1 + mi] * y;
            this->data[8 + smi] = this->data[2 + mi] * z;
            this->data[12 + smi] = this->data[3 + mi];
        }
    }

    void Matrix4x4::makeScale(const Vector3Components<Real>& scale) {
        this->makeScale(scale.x, scale.y, scale.z);
    }

    void Matrix4x4::makeScale(Real x, Real y, Real z) {
        this->setIdentity();
        this->data[0] = x;
        this->data[5] = y;
        this->data[10] = z;
    }

    void Matrix4x4::lookAt(const Vector3Components<Real> &src, const Vector3Components<Real> &target, const Vector3Components<Real> &up) {
        Point3r _src(src.x, src.y, src.z);
        Point3r _target(target.x, target.y, target.z);
        Vector3r toTarget = _target - _src;
        toTarget.normalize();

        Vector3r vUp(up.x, up.y, up.z);
        Vector3r vRight;

        Vector3r::cross(toTarget, vUp, vRight);
        vRight.normalize();

        Vector3r::cross(vRight, toTarget, vUp);
        vUp.normalize();

        this->data[0] = vRight.x;
        this->data[1] = vRight.y;
        this->data[2] = vRight.z;
        this->data[3] = 0.0f;

        this->data[4] = vUp.x;
        this->data[5] = vUp.y;
        this->data[6] = vUp.z;
        this->data[7] = 0.0f;

        this->data[8] = -toTarget.x;
        this->data[9] = -toTarget.y;
        this->data[10] = -toTarget.z;
        this->data[11] = 0.0f;

        this->data[12] = src.x;
        this->data[13] = src.y;
        this->data[14] = src.z;
        this->data[15] = 1.0f;
    }
}
