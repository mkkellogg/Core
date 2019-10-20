#include "TranslationKeyFrame.h"

namespace Core {
	/*
	* Default constructor.
	*/
	TranslationKeyFrame::TranslationKeyFrame() : KeyFrame() {

	}

	/*
	 * Parameterized constructor..
	 */
	TranslationKeyFrame::TranslationKeyFrame(Real normalizedTime, Real realTime, Real realTimeTicks, const Vector3r& translation) : KeyFrame(normalizedTime, realTime, realTimeTicks) {
		this->Translation = translation;
	}

	/*
	 * Destructor.
	 */
	TranslationKeyFrame::~TranslationKeyFrame() {

	}
}
