/*********************************************
*
* class: KeyFrame
*
* author: Mark Kellogg
*
* A single key frame in an animation. This class serves as the
* base class for TranslationKeyFrame, ScaleKeyFrame, and RotationKeyFrame.
*
***********************************************/

#pragma once

#include "../Engine.h"
#include "../common/types.h"

namespace Core {
	class KeyFrame {
	public:

		// the time in the animation of this key frame, in seconds
		Real RealTime;
		// the time of this key frame in the animation, normalized to the range 0-1
		Real NormalizedTime;
		// the time of this key frame in the animation in ticks
		Real RealTimeTicks;

		KeyFrame();
		KeyFrame(Real normalizedTime, Real realTime, Real realTimeTicks);
		virtual ~KeyFrame();
	};
}
