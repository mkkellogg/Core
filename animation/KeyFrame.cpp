#include "KeyFrame.h"

namespace Core {
	/*
	* Default constructor.
	*/
	KeyFrame::KeyFrame() {
		NormalizedTime = 0;
		RealTime = 0;
		RealTimeTicks = 1;
	}

	/*
	 * Parameterized constructor.
	 */
	KeyFrame::KeyFrame(Real normalizedTime, Real realTime, Real realTimeTicks) {
		this->NormalizedTime = normalizedTime;
		this->RealTime = realTime;
		this->RealTimeTicks = realTimeTicks;
	}

	/*
	 * Destructor.
	 */
	KeyFrame::~KeyFrame() {
	}
}
