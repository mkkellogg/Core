#include "CrossFadeBlendOp.h"
#include "../util/Time.h"

namespace Core {

	/*
	* Default constructor.
	*/
	CrossFadeBlendOp::CrossFadeBlendOp(Real duration, UInt32 targetIndex) : BlendOp(duration) {
		this->targetIndex = targetIndex;
		this->setComplete(false);
		if (duration <= 0) this->setComplete(true);
	}

	/*
	 * Destructor.
	 */
	CrossFadeBlendOp::~CrossFadeBlendOp() {
	}

	/*
	 *Adjust the relevant weights of animation player.
	 */
	void CrossFadeBlendOp::update(std::vector<Real>& weights) {

		if (this->complete) return;
		if (weights.size() < this->initialWeights.size()) {
			throw Exception("CrossFadeBlendOp::Update -> Weight count is less than initial weight count.");
		}

		BlendOp::update(weights);
		Real normalizedProgress = progress / duration;

		for (UInt32 i = 0; i < this->initialWeights.size(); i++) {
			Real initialWeight = this->initialWeights[i];
			Real targetWeight = 0;
			if (i == targetIndex)targetWeight = 1;
			Real deltaWeight = targetWeight - initialWeight;
			weights[i] = normalizedProgress * deltaWeight + initialWeight;
		}

		if (progress > duration) {
			for (UInt32 i = 0; i < this->initialWeights.size(); i++)
			{
				if (i == targetIndex) weights[i] = 1;
				else weights[i] = 0;
			}
			this->setComplete(true);
		}
	}

	void CrossFadeBlendOp::onStart() {
		if (this->startCallback) this->startCallback(this);
	}

	void CrossFadeBlendOp::onComplete() {
		if (this->completeCallback) this->completeCallback(this);
	}

	void CrossFadeBlendOp::onStoppedEarly() {
		if (this->stoppedEarlyCallback) this->stoppedEarlyCallback(this);
	}

	void CrossFadeBlendOp::setOnStartCallback(std::function<void(CrossFadeBlendOp*)> callback) {
		this->startCallback = callback;
	}

	void CrossFadeBlendOp::setOnCompleteCallback(std::function<void(CrossFadeBlendOp*)> callback) {
		this->completeCallback = callback;
	}

	void CrossFadeBlendOp::setOnStoppedEarlyCallback(std::function<void(CrossFadeBlendOp*)> callback) {
		this->stoppedEarlyCallback = callback;
	}

	/*
	 * Get the target animation index for this cross fade operation.
	 */
	UInt32 CrossFadeBlendOp::getTargetIndex() const {
		return this->targetIndex;
	}
}

