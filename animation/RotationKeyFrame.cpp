#include "RotationKeyFrame.h"

namespace Core {
	
	/*
	* Default constructor.
	*/
	RotationKeyFrame::RotationKeyFrame() : KeyFrame() {
	}

	/*
	 * Parameterized constructor.
	 */
	RotationKeyFrame::RotationKeyFrame(Real normalizedTime, Real realTime, Real realTimeTicks, const Quaternion& rotation) : KeyFrame(normalizedTime, realTime, realTimeTicks) {
		this->Rotation = rotation;
	}

	/*
	 * Destructor.
	 */
	RotationKeyFrame::~RotationKeyFrame() {
	}
}
