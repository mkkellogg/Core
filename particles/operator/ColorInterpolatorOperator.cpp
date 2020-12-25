#include <functional>

#include "../../color/Color.h"
#include "ColorInterpolatorOperator.h"
#include "../../util/Time.h"

namespace Core {

    ColorInterpolatorOperator::ColorInterpolatorOperator(Bool ignoreAlpha, Bool relativeToInitialValue): InterpolatorOperator(relativeToInitialValue) {
        this->ignoreAlpha = ignoreAlpha;
    }

    ColorInterpolatorOperator::~ColorInterpolatorOperator() {
    }

    Bool ColorInterpolatorOperator::updateState(ParticleStatePtr& state, Real timeDelta) {
        Color c;
        this->getInterpolatedValue(state, c);
        if (this->relativeToInitialValue) {
            state.color->r = state.initialColor->r * c.r;
            state.color->g = state.initialColor->g * c.g;
            state.color->b = state.initialColor->b * c.b;
            if(!ignoreAlpha) state.color->a = state.initialColor->a * c.a;
        } else {
            Real a = state.color->a;
            state.color->set(c.r, c.g, c.b, this->ignoreAlpha ? a : c.a);
        }
        return true;
    }
}
