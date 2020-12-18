#pragma once

#include "../../util/PersistentWeakPointer.h"
#include "../../common/types.h"
#include "ParticleStateOperator.h"
#include "../ParticleState.h"
#include "../../util/ContinuousArray.h"

namespace Core {

    template <typename T>
    class InterpolatorOperator: public ParticleStateOperator {
    public:
        InterpolatorOperator() {}
        virtual ~InterpolatorOperator() {}

        void addElement(const T& element, Real tValue) {
            this->interpolationElements.addElement(element, tValue);
        }

    protected:

        void getInterpolatedValue(ParticleStatePtr& state, T& out) {
            Real t = *state.age / *state.lifetime;
            this->interpolationElements.getInterpolatedElement(t, out);
        }

        ContinuousArray<T> interpolationElements;
    };
}