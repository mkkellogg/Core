#include "SizeInitializer.h"
#include "../../math/Math.h"

namespace Core {

    SizeInitializer::SizeInitializer(const Generator<Vector2r>& generator) {
        this->generator = std::unique_ptr<Generator<Vector2r>>(generator.clone());
    }

    SizeInitializer::~SizeInitializer() {
    }

    void SizeInitializer::initializeState(ParticleStatePtr& state) {
        Vector2r size;
        this->generator->generate(size);
        state.initialSize->copy(size);
        state.size->copy(size);
    }

}
