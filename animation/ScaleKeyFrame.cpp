#include "ScaleKeyFrame.h"

namespace Core {

	/*
	* Default constructor.
	*/
	ScaleKeyFrame::ScaleKeyFrame() : KeyFrame() {
	}

	/*
	 * Parameterized constructor.
	 */
	ScaleKeyFrame::ScaleKeyFrame(Real normalizedTime, Real realTime, Real realTimeTicks, const Vector3r& scale) : KeyFrame(normalizedTime, realTime, realTimeTicks) {
		this->Scale = scale;
	}

	/*
	 * Destructor.
	 */
	ScaleKeyFrame::~ScaleKeyFrame() {
	}
}
