#include "ContinuousArray.h"
#include "../geometry/Vector3.h"
#include "../geometry/Vector2.h"
#include "../color/Color.h"

namespace Core {

    template <>
    typename ContinuousArray<Real>::Interpolator ContinuousArray<Real>::getInterpolator() {
        return [this](Real tValue, std::vector<Real>& elements, std::vector<Real>& tValues, Real& out) {
            Int32 lowerIndex, upperIndex;
            Real localT;
            this->getInterpolationValuesForTValue(tValue, lowerIndex, upperIndex, localT); 
            out = (1.0f - localT) * this->elements[lowerIndex] + localT * this->elements[upperIndex];
        };
    }

    template <>
    typename ContinuousArray<Vector2r>::Interpolator ContinuousArray<Vector2r>::getInterpolator() {
        return [this](Real tValue, std::vector<Vector2r>& elements, std::vector<Real>& tValues, Vector2r& out) {
            Int32 lowerIndex, upperIndex;
            Real localT;
            this->getInterpolationValuesForTValue(tValue, lowerIndex, upperIndex, localT);
            out.copy(this->elements[lowerIndex]);
            Vector2r upper;
            upper.copy(this->elements[upperIndex]);
            out.set(out.x * (1.0f - localT)  + localT * upper.x,
                    out.y * (1.0f - localT)  + localT * upper.y);
            
        };
    }

    template <>
    typename ContinuousArray<Vector3r>::Interpolator ContinuousArray<Vector3r>::getInterpolator() {
        return [this](Real tValue, std::vector<Vector3r>& elements, std::vector<Real>& tValues, Vector3r& out) {
            Int32 lowerIndex, upperIndex;
            Real localT;
            this->getInterpolationValuesForTValue(tValue, lowerIndex, upperIndex, localT);
            out.copy(this->elements[lowerIndex]);
            Vector3r upper;
            upper.copy(this->elements[upperIndex]);
            out.set(out.x * (1.0f - localT)  + localT * upper.x,
                    out.y * (1.0f - localT)  + localT * upper.y,
                    out.z * (1.0f - localT)  + localT * upper.z);
            
        };
    }

    template <>
    typename ContinuousArray<Color>::Interpolator ContinuousArray<Color>::getInterpolator() {
        return [this](Real tValue, std::vector<Color>& elements, std::vector<Real>& tValues, Color& out) {
            Int32 lowerIndex, upperIndex;
            Real localT;
            this->getInterpolationValuesForTValue(tValue, lowerIndex, upperIndex, localT);
            out.copy(this->elements[lowerIndex]);
            Color upper;
            upper.copy(this->elements[upperIndex]);
            //std::cerr << std::to_string(this->elements.size()) << ", " << std::to_string(lowerIndex) << ", " << std::to_string(upperIndex) << std::endl;;
            out.set(out.r * (1.0f - localT)  + localT * upper.r,
                    out.g * (1.0f - localT)  + localT * upper.g,
                    out.b * (1.0f - localT)  + localT * upper.b,
                    out.a * (1.0f - localT)  + localT * upper.a);
            //std::cerr << std::to_string(this->elements[1].r) << ", " << std::to_string(this->elements[1].g) << ", " << std::to_string(this->elements[1].b) << ", " << std::to_string(this->elements[1].a) << std::endl;
            //out.copy(this->elements[1]);
        };
    }

}