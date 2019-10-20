#include "BlendOp.h"
#include "../util/Time.h"

namespace Core {

	/*
	 * Default constructor.
	 */
	BlendOp::BlendOp(Real duration) {
		this->duration = duration;
		this->progress = 0;
		complete = false;
		started = false;
	}

	/*
	 * Destructor.
	 */
	BlendOp::~BlendOp() {
	}

	Bool BlendOp::init(const std::vector<Real>& initialWeights) {
		this->initialWeights = initialWeights;
		this->reset();
		return true;
	}

	void BlendOp::reset() {
		this->progress = 0;
		this->complete = false;
	}

	void BlendOp::update(std::vector<Real>& weights) {
		progress += Time::getDeltaTime();
	}

	void BlendOp::setStarted(Bool started) {
		this->started = started;
	}

	void BlendOp::setComplete(Bool complete) {
		this->complete = complete;
	}

	Bool BlendOp::hasStarted() const {
		return started;
	}

	Bool BlendOp::hasCompleted() const {
		return complete;
	}

	Real BlendOp::getNormalizedProgress() const {
		return progress / duration;
	}

	Real BlendOp::getProgress() const {
		return progress;
	}
}

