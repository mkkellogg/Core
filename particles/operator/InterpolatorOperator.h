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
        InterpolatorOperator(Bool relativeToInitialValue = false) {
            this->relativeToInitialValue = relativeToInitialValue;
        }
        virtual ~InterpolatorOperator() {}

        void addElement(const T& element, Real tValue) {
            this->interpolationElements.addElement(element, tValue);
        }

    protected:

        void getInterpolatedValue(ParticleStatePtr& state, T& out) {
            Real t = 0.0f;
            switch((ParticleStateProgressType)(UInt32)*state.progressType) {
                case ParticleStateProgressType::Time:
                {
                    Real lifetime = *state.lifetime;
                    if (lifetime != 0.0f) {
                        t = *state.age / *state.lifetime;
                    } else {
                        t = *state.age;
                    }
                }
                break;
                case ParticleStateProgressType::Sequence:
                    t = (*state.sequenceElement).x / (*state.sequenceElement).z;
                break;
            }
            this->interpolationElements.getInterpolatedElement(t, out);
        }

        ContinuousArray<T> interpolationElements;
        Bool relativeToInitialValue;
    };
}