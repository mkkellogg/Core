#include <cmath>

#include "Quaternion.h"
#include "../common/Exception.h"
#include "../common/debug.h"
#include "../geometry/Vector3.h"
#include "Math.h"
#include "Matrix4x4.h"


namespace Core {

    const Quaternion Quaternion::Identity(0, 0, 0, 1);

    Quaternion::Quaternion() {
        mData[0] = mData[1] = mData[2] = 0;
        mData[3] = 1;
    }

    Quaternion::Quaternion(const Real* array) {
        if (array == nullptr) {
            throw NullPointerException("Quaternion() -> 'array' is null!");
        }
        for (UInt32 i = 0; i < 4; i++) {
            mData[i] = array[i];
        }
    }

    Quaternion::Quaternion(Real x, Real y, Real z, Real w) {
        mData[0] = x;
        mData[1] = y;
        mData[2] = z;
        mData[3] = w;
    }

    void Quaternion::set(Real x, Real y, Real z, Real w) {
        mData[0] = x;
        mData[1] = y;
        mData[2] = z;
        mData[3] = w;
    }

    Quaternion::Quaternion(const Matrix4x4& matrix) {
        this->fromMatrix(matrix);
    }

    Quaternion::Quaternion(const Vector3Components<Real>& v, Real w) {
        mData[0] = v.x;
        mData[1] = v.y;
        mData[2] = v.z;
        mData[3] = w;
    };

    Real Quaternion::x() const {
        return mData[0];
    }

    Real Quaternion::y() const {
        return mData[1];
    }

    Real Quaternion::z() const {
        return mData[2];
    }

    Real Quaternion::w() const {
        return real();
    }

    Vector3r Quaternion::complex() const {
        return Vector3r(mData[0], mData[1], mData[2]);
    }

    void Quaternion::complex(const Vector3Components<Real>& c) {
        mData[0] = c.x;
        mData[1] = c.y;
        mData[2] = c.z;
    }

    Real Quaternion::real() const {
        return mData[3];
    }

    void Quaternion::real(Real r) {
        mData[3] = r;
    }

    Quaternion Quaternion::conjugate(void) const {
        Vector3r comp = complex();
        comp.x = -comp.x;
        comp.y = -comp.y;
        comp.z = -comp.z;
        return Quaternion(comp, real());
    }

    /**
     * Computes the inverse of this quaternion.
     *
     * @note This is a general inverse.  If you know a priori
     * that you're using a unit quaternion (i.e., norm() == 1),
     * it will be significantly faster to use conjugate() instead.
     *
     * @return The quaternion q such that q * (*this) == (*this) * q
     * == [ 0 0 0 1 ]<sup>T</sup>.
     */
    Quaternion Quaternion::inverse(void) const {
        return conjugate() / norm();
    }

    /**
     * Computes the product of this quaternion with the
     * quaternion 'rhs'.
     *
     * @param rhs The right-hand-side of the product operation.
     *
     * @return The quaternion product (*this) x @p rhs.
     */
    Quaternion Quaternion::product(const Quaternion& rhs) const {
        return Quaternion(y() * rhs.z() - z() * rhs.y() + x() * rhs.w() + w() * rhs.x(), z() * rhs.x() - x() * rhs.z() + y() * rhs.w() + w() * rhs.y(),
                          x() * rhs.y() - y() * rhs.x() + z() * rhs.w() + w() * rhs.z(), w() * rhs.w() - x() * rhs.x() - y() * rhs.y() - z() * rhs.z());
    }

    /**
     * Quaternion product operator.
     *
     * The result is a quaternion such that:
     *
     * result.real() = (*this).real() * rhs.real() -
     * (*this).complex().dot(rhs.complex());
     *
     * and:
     *
     * result.complex() = rhs.complex() * (*this).real
     * + (*this).complex() * rhs.real()
     * - (*this).complex().cross(rhs.complex());
     *
     * @return The quaternion product (*this) x rhs.
     */
    Quaternion Quaternion::operator*(const Quaternion& rhs) const {
        return product(rhs);
    }

    /**
     * Quaternion scalar product operator.
     * @param s A scalar by which to multiply all components
     * of this quaternion.
     * @return The quaternion (*this) * s.
     */
    /* Quaternion Quaternion::operator*(Real s) const
     {
       Vector3 comp = complex();
       comp.x *= s;
       comp.y *= s;
       comp.z *= s;
       return Quaternion(comp, real() * s);
     }*/

    /**
     * Produces the sum of this quaternion and rhs.
     */
    Quaternion Quaternion::operator+(const Quaternion& rhs) const {
        return Quaternion(x() + rhs.x(), y() + rhs.y(), z() + rhs.z(), w() + rhs.w());
    }

    /**
     * Produces the difference of this quaternion and rhs.
     */
    Quaternion Quaternion::operator-(const Quaternion& rhs) const {
        return Quaternion(x() - rhs.x(), y() - rhs.y(), z() - rhs.z(), w() - rhs.w());
    }

    /**
     * Unary negation.
     */
    Quaternion Quaternion::operator-() const {
        return Quaternion(-x(), -y(), -z(), -w());
    }

    /**
     * Quaternion scalar division operator.
     * @param s A scalar by which to divide all components
     * of this quaternion.
     * @return The quaternion (*this) / s.
     */
    Quaternion Quaternion::operator/(Real s) const {
        if (s == 0) Debug::PrintMessage("Dividing quaternion by 0.\n");
        Vector3r comp = complex();
        comp.x /= s;
        comp.y /= s;
        comp.z /= s;
        return Quaternion(comp, real() / s);
    }

    /**
     * Returns a matrix representation of this
     * quaternion.
     *
     * Specifically this is the matrix such that:
     *
     * this->matrix() * q.vector() = (*this) * q for any quaternion q.
     *
     * Note that this is @e NOT the rotation matrix that may be
     * represented by a unit quaternion.
     */
    Matrix4x4 Quaternion::matrix() const {
        Real m[16] = {w(), -z(), y(), x(), z(), w(), -x(), y(), -y(), x(), w(), z(), -x(), -y(), -z(), w()};

        return Matrix4x4(m);
    }

    /**
     * Returns a matrix representation of this
     * quaternion for right multiplication.
     *
     * Specifically this is the matrix such that:
     *
     * q.vector().transpose() * this->matrix() = (q *
     * (*this)).vector().transpose() for any quaternion q.
     *
     * Note that this is @e NOT the rotation matrix that may be
     * represented by a unit quaternion.
     */
    Matrix4x4 Quaternion::rightMatrix() const {
        Real m[16] = {+w(), -z(), y(), -x(), +z(), w(), -x(), -y(), -y(), x(), w(), -z(), +x(), y(), z(), w()};
        return Matrix4x4(m);
    }

    Vector3r Quaternion::vector() const {
        return complex();
    }

    /**
     * Returns this quaternion as a 4-vector.
     *
     * This is simply the vector [x y z w]<sup>T</sup>
     */
    // TVector4 vector() const { return TVector4(mData); }

    void Quaternion::normalize() {
        Real normFactor = norm();
        mData[0] /= normFactor;
        mData[1] /= normFactor;
        mData[2] /= normFactor;
        mData[3] /= normFactor;
    }

    /**
     * Returns the norm ("magnitude") of the quaternion.
     * @return The 2-norm of [ w(), x(), y(), z() ]<sup>T</sup>.
     */
    Real Quaternion::norm() const {
        return Math::SquareRoot(mData[0] * mData[0] + mData[1] * mData[1] + mData[2] * mData[2] + mData[3] * mData[3]);
    }

    void Quaternion::setRotationMatrix(Real* data) const {
        data[0] = 1 - 2 * y() * y() - 2 * z() * z();
        data[1] = 2 * x() * y() + 2 * z() * w();
        data[2] = 2 * x() * z() - 2 * y() * w();
        data[3] = 0;

        data[4] = 2 * x() * y() - 2 * z() * w();
        data[5] = 1 - 2 * x() * x() - 2 * z() * z();
        data[6] = 2 * y() * z() + 2 * x() * w();
        data[7] = 0;

        data[8] = 2 * x() * z() + 2 * y() * w();
        data[9] = 2 * y() * z() - 2 * x() * w();
        data[10] = 1 - 2 * x() * x() - 2 * y() * y();
        data[11] = 0;

        data[12] = 0;
        data[13] = 0;
        data[14] = 0;
        data[15] = 1;
    }

    /**
     * Computes the rotation matrix represented by a unit
     * quaternion.
     *
     * @note This does not check that this quaternion is normalized.
     * It formulaically returns the matrix, which will not be a
     * rotation if the quaternion is non-unit.
     */
    Matrix4x4 Quaternion::rotationMatrix() const {
        Real m[SIZE_MATRIX_4X4];
        Quaternion::setRotationMatrix(m);
        Matrix4x4 p(m);
        return p;
    }

    void Quaternion::rotationMatrix(Matrix4x4& out) const {
        Real* outData = const_cast<Real*>(out.getData());

        outData[0] = 1 - 2 * y() * y() - 2 * z() * z();
        outData[1] = 2 * x() * y() + 2 * z() * w();
        outData[2] = 2 * x() * z() - 2 * y() * w();
        outData[3] = 0;

        outData[4] = 2 * x() * y() - 2 * z() * w();
        outData[5] = 1 - 2 * x() * x() - 2 * z() * z();
        outData[6] = 2 * y() * z() + 2 * x() * w();
        outData[7] = 0;

        outData[8] = 2 * x() * z() + 2 * y() * w();
        outData[9] = 2 * y() * z() - 2 * x() * w();
        outData[10] = 1 - 2 * x() * x() - 2 * y() * y();
        outData[11] = 0;

        outData[12] = 0;
        outData[13] = 0;
        outData[14] = 0;
        outData[15] = 1;
    }

    /*
     * Based off the function Quaternion::fromRotationMatrix in the Ogre open source engine.
     */
    void Quaternion::fromMatrix(const Matrix4x4& matrix) {
        Real trace = matrix.A0 + matrix.B1 + matrix.C2;
        Real root;

        const Real* data = matrix.getConstData();

        if (trace > 0.0) {
            root = Math::SquareRoot(trace + 1.0f);
            mData[3] = 0.5f * root;
            root = 0.5f / root;
            mData[0] = (matrix.C1 - matrix.B2) * root;
            mData[1] = (matrix.A2 - matrix.C0) * root;
            mData[2] = (matrix.B0 - matrix.A1) * root;
        } else {
            static UInt32 iNext[3] = {1, 2, 0};
            UInt32 i = 0;
            if (matrix.B1 > matrix.A0) i = 1;
            if (matrix.C2 > data[i * 4 + i]) i = 2;
            UInt32 j = iNext[i];
            UInt32 k = iNext[j];

            root = Math::SquareRoot(data[i * 4 + i] - data[j * 4 + j] - data[k * 4 + k] + 1.0f);
            Real* apkQuat[3] = {mData, mData + 1, mData + 2};
            *apkQuat[i] = 0.5f * root;
            root = 0.5f / root;

            mData[3] = (data[j * 4 + k] - data[k * 4 + j]) * root;
            *apkQuat[j] = (data[i * 4 + j] + data[j * 4 + i]) * root;
            *apkQuat[k] = (data[i * 4 + k] + data[k * 4 + i]) * root;

            // w = (kRot[k][j]-kRot[j][k])*root;
            //*apkQuat[j] = (kRot[j][i]+kRot[i][j])*root;
            //*apkQuat[k] = (kRot[k][i]+kRot[i][k])*root;
        }
    }

    void Quaternion::fromAngleAxis(const Real rfAngle, const Vector3Components<Real>& rkAxis) {
        // assert:  axis[] is unit length
        //
        // The quaternion representing the rotation is
        //   q = cos(A/2)+sin(A/2)*(x*i+y*j+z*k)

        Real fHalfAngle(0.5f * rfAngle);
        Real fSin = Math::Sin(fHalfAngle);
        mData[3] = Math::Cos(fHalfAngle);
        mData[0] = fSin * rkAxis.x;
        mData[1] = fSin * rkAxis.y;
        mData[2] = fSin * rkAxis.z;
    }

    void Quaternion::fromAngleAxis(Real angle, Real x, Real y, Real z) {
        // assert:  axis[] is unit length
        //
        // The quaternion representing the rotation is
        //   q = cos(A/2)+sin(A/2)*(x*i+y*j+z*k)

        Real fHalfAngle(0.5f * angle);
        Real fSin = Math::Sin(fHalfAngle);
        mData[3] = Math::Cos(fHalfAngle);
        mData[0] = fSin * x;
        mData[1] = fSin * y;
        mData[2] = fSin * z;
    }

    /**
     * Sets quaternion to be same as rotation by scaled axis w.
     */
    /*void scaledAxis(const Vector3Components<Real>& w) {
      Real theta = w.length();
      if (theta > 0.0001) {
        Real s = Math::Sin(theta / 2.0);
        Vector3Components<Real> W(w / theta * s);
        mData[0] = W[0];
        mData[1] = W[1];
        mData[2] = W[2];
        mData[3] = Math::Cos(theta / 2.0);
        } else {
        mData[0]= mData[1] = mData[2] = 0;
        mData[3]= 1.0;
      }
    }*/

    /**
     * Returns a vector rotated by this quaternion.
     *
     * Functionally equivalent to:  (rotationMatrix() * v)
     * or (q * Quaternion(0, v) * q.inverse()).
     *
     * @warning conjugate() is used instead of inverse() for better
     * performance, when this quaternion must be normalized.
     */
    Vector3r Quaternion::rotatedVector(const Vector3Components<Real>& v) const {
        return (((*this) * Quaternion(v, 0)) * conjugate()).complex();
    }

    /**
     * Computes the quaternion that is equivalent to a given
     * euler angle rotation.
     * @param euler A 3-vector in order:  roll-pitch-yaw.
     */
    void Quaternion::euler(const Vector3Components<Real>& euler) {
        Real c1 = Math::Cos(euler.z * 0.5f * Math::DegreesToRads);
        Real c2 = Math::Cos(euler.y * 0.5f * Math::DegreesToRads);
        Real c3 = Math::Cos(euler.x * 0.5f * Math::DegreesToRads);
        Real s1 = Math::Sin(euler.z * 0.5f * Math::DegreesToRads);
        Real s2 = Math::Sin(euler.y * 0.5f * Math::DegreesToRads);
        Real s3 = Math::Sin(euler.x * 0.5f * Math::DegreesToRads);

        mData[0] = c1 * c2 * s3 - s1 * s2 * c3;
        mData[1] = c1 * s2 * c3 + s1 * c2 * s3;
        mData[2] = s1 * c2 * c3 - c1 * s2 * s3;
        mData[3] = c1 * c2 * c3 + s1 * s2 * s3;
    }

    /** Returns an equivalent euler angle representation of
     * this quaternion.
     * @return Euler angles in roll-pitch-yaw order.
     */
    Vector3r Quaternion::euler(void) const {
        Vector3r euler;
        const static Real PI_OVER_2 = Math::PI * 0.5f;
        const static Real EPSILON = (Real)1e-10;
        Real sqw, sqx, sqy, sqz;

        // quick conversion to Euler angles to give tilt to user
        sqw = mData[3] * mData[3];
        sqx = mData[0] * mData[0];
        sqy = mData[1] * mData[1];
        sqz = mData[2] * mData[2];

        euler.y = asin(2.0f * (mData[3] * mData[1] - mData[0] * mData[2]));
        if (PI_OVER_2 - fabs(euler.y) > EPSILON) {
            euler.z = atan2(2.0f * (mData[0] * mData[1] + mData[3] * mData[2]), sqx - sqy - sqz + sqw);
            euler.x = atan2(2.0f * (mData[3] * mData[0] + mData[1] * mData[2]), sqw - sqx - sqy + sqz);
        } else {
            // compute heading from local 'down' vector
            euler.z = atan2(2 * mData[1] * mData[2] - 2 * mData[0] * mData[3], 2 * mData[0] * mData[2] + 2 * mData[1] * mData[3]);
            euler.x = 0.0f;

            // If facing down, reverse yaw
            if (euler.y < 0) euler.z = Math::PI - euler.z;
        }
        return euler;
    }

    /**
     * Computes a special representation that decouples the Z
     * rotation.
     *
     * The decoupled representation is two rotations, Qxy and Qz,
     * so that Q = Qxy * Qz.
     */
    /*void decoupleZ(Quaternion* Qxy, Quaternion* Qz) const
     {
     TVector3 ztt(0,0,1);
     TVector3 zbt = this->rotatedVector(ztt);
     TVector3 axis_xy = ztt.cross(zbt);
     Real axis_norm = axis_xy.norm();

     Real axis_theta = acos(HeliMath::saturate(zbt[2], -1,+1));
     if (axis_norm > 0.00001) {
     axis_xy = axis_xy * (axis_theta/axis_norm); // limit is *1
     }

     Qxy->scaledAxis(axis_xy);
     *Qz = (Qxy->conjugate() * (*this));
     }*/

    /**
     * Returns the quaternion slerped between this and q1 by fraction 0 <= t <= 1.
     */
    Quaternion Quaternion::slerp(const Quaternion& q1, Real t) {
        return slerp(*this, q1, t);
    }

    Quaternion Quaternion::slerp(const Quaternion& qa, const Quaternion& qb, Real t) {
        /*  // quaternion to return
          Quaternion qm;
          // Calculate angle between them.
          Real cosHalfTheta = qa.w() * qb.w() + qa.x() * qb.x() + qa.y() * qb.y() + qa.z() * qb.z();
          // if qa=qb or qa=-qb then theta = 0 and we can return qa
          if (abs(cosHalfTheta) >= 1.0)
          {
          qm.mData[3] = qa.w();
          qm.mData[0] = qa.x();
          qm.mData[1] = qa.y();
          qm.mData[2] = qa.z();
          return qm;
          }
          // Calculate temporary values.
          Real halfTheta = acos(cosHalfTheta);
          Real sinHalfTheta = sqrt(1.0 - cosHalfTheta * cosHalfTheta);
          // if theta = 180 degrees then result is not fully defined
          // we could rotate around any axis normal to qa or qb
          if (fabs(sinHalfTheta) < 0.001)
          { // fabs is floating point absolute
          qm.mData[3] = (qa.w() * 0.5 + qb.w() * 0.5);
          qm.mData[0] = (qa.x() * 0.5 + qb.x() * 0.5);
          qm.mData[1] = (qa.y() * 0.5 + qb.y() * 0.5);
          qm.mData[2] = (qa.z() * 0.5 + qb.z() * 0.5);
          return qm;
          }
          Real ratioA = sin((1 - t) * halfTheta) / sinHalfTheta;
          Real ratioB = sin(t * halfTheta) / sinHalfTheta;
          //calculate Quaternion.
          qm.mData[3] = (qa.w() * ratioA + qb.w() * ratioB);
          qm.mData[0] = (qa.x() * ratioA + qb.x() * ratioB);
          qm.mData[1] = (qa.y() * ratioA + qb.y() * ratioB);
          qm.mData[2] = (qa.z() * ratioA + qb.z() * ratioB);
          return qm;*/

        // calc cosine theta
        Real cosom = qa.x() * qb.x() + qa.y() * qb.y() + qa.z() * qb.z() + qa.w() * qb.w();
        // adjust signs (if necessary)
        Quaternion end = qb;
        if (cosom < static_cast<Real>(0.0)) {
            cosom = -cosom;
            end.mData[0] = -end.x();  // Reverse all signs
            end.mData[1] = -end.y();
            end.mData[2] = -end.z();
            end.mData[3] = -end.w();
        }
        // Calculate coefficients
        Real sclp, sclq;
        if ((static_cast<Real>(1.0) - cosom) > static_cast<Real>(0.0001))  // 0.0001 -> some epsillon
        {
            // Standard case (slerp)
            Real omega, sinom;
            omega = Math::ACos(cosom);  // extract theta from dot product's cos theta
            sinom = Math::Sin(omega);
            sclp = Math::Sin((static_cast<Real>(1.0) - t) * omega) / sinom;
            sclq = Math::Sin(t * omega) / sinom;
        } else {
            // Very close, do linear interp (because it's faster)
            sclp = static_cast<Real>(1.0) - t;
            sclq = t;
        }

        Quaternion qOut;

        Real x = sclp * qa.x() + sclq * end.x();
        Real y = sclp * qa.y() + sclq * end.y();
        Real z = sclp * qa.z() + sclq * end.z();
        Real w = sclp * qa.w() + sclq * end.w();

        qOut.set(x, y, z, w);
        return qOut;
    }

    /**
     * Returns pointer to the internal array.
     *
     * Array is in order x,y,z,w.
     */
    Real* Quaternion::row(UInt32 i) {
        return mData + i;
    }
    // Const version of the above.
    const Real* Quaternion::row(UInt32 i) const {
        return mData + i;
    }

    Quaternion Quaternion::getRotation(const Vector3Components<Real>& source, const Vector3Components<Real>& dest) {
        return getRotation(source, dest, Vector3r::Zero);
    }

    Quaternion Quaternion::getRotation(const Vector3Components<Real>& source, const Vector3Components<Real>& dest,
                                       const Vector3Components<Real>& fallbackAxis) {
        // Based on Stan Melax's article in Game Programming Gems
        Quaternion q;

        // Copy, since cannot modify local
        Vector3r v0;
        v0.copy(source);

        Vector3r v1;
        v1.copy(dest);

        v0.normalize();
        v1.normalize();

        Real d = Vector3r::dot(v0, v1);

        Vector3r fallbackAxisV3;
        fallbackAxisV3.copy(fallbackAxis);

        // If dot == 1, vectors are the same
        if (d >= 1.0f) {
            return Quaternion::Identity;
        }

        Vector3r axis;
        if (d < (1e-6f - 1.0f)) {
            if (!(fallbackAxisV3 == Vector3r::Zero)) {
                // rotate 180 degrees about the fallback axis
                q.fromAngleAxis(Math::PI, fallbackAxis);
            } else {
                // Generate an axis
                Vector3r::cross(Vector3r::UnitX, v0, axis);

                if (axis.isZeroLength())  // pick another if colinear
                {
                    Vector3r::cross(Vector3r::UnitY, v0, axis);
                }

                axis.normalize();
                q.fromAngleAxis(Math::PI, axis);
            }
        } else {
            Real s = Math::SquareRoot((1 + d) * 2);
            Real invs = 1 / s;

            Vector3r c;
            Vector3r::cross(v0, v1, c);

            q.set(c.x * invs, c.y * invs, c.z * invs, s * 0.5f);
            q.normalize();
        }

        return q;
    }

    /**
     * Global operator allowing left-multiply by scalar.
     */
    Quaternion operator*(Real s, const Quaternion& q);
}
