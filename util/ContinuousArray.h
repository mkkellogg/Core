#pragma once

#include <vector>
#include <functional>

#include "PersistentWeakPointer.h"
#include "../math/Math.h"

namespace Core {

    template <typename T>
    class ContinuousArray {
    public:

        typedef std::function<void(Real, std::vector<T>&, std::vector<Real>&, T& out)> Interpolator;

        ContinuousArray(Interpolator interpolator) {
            this->interpolator = interpolator;
        }

        ContinuousArray() {
            this->interpolator = this->getInterpolator();
        }

        ~ContinuousArray() {
        }

        void addElement(const T& element, Real tValue) {
            this->elements.push_back(element);
            this->tValues.push_back(tValue);
        }

        void getInterpolatedElement(Real t, T& out) {
            this->interpolator(t, this->elements, this->tValues, out);
        }

    private:

        ContinuousArray<T>::Interpolator getInterpolator();
        void getInterpolationValuesForTValue(Real t, UInt32& lowerIndex, UInt32& upperIndex, Real& localT) {
            UInt32 tValueCount = this->tValues.size();
            if (tValueCount == 0) {
                lowerIndex = upperIndex = -1;
                localT = -1.0f;
                return;
            }
            Real tValue = 0.0f;
            lowerIndex = -1;
            upperIndex = 0;
            for (UInt32 i = 0; i < tValueCount; i++) {
                tValue = this->tValues[i];
                if (tValue > t) break;
                lowerIndex++;
                upperIndex++;
            }
            lowerIndex = Math::clamp(lowerIndex, (UInt32)0, tValueCount - 1);
            upperIndex = Math::clamp(upperIndex, (UInt32)0, tValueCount - 1);
            Real lowerTValue = this->tValues[lowerIndex];
            Real upperTValue = this->tValues[upperIndex];
            localT = (t - lowerTValue) / (upperTValue - lowerTValue);
        }

        std::vector<T> elements;
        std::vector<Real> tValues;
        Interpolator interpolator;
    };

}