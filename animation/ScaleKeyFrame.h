/*********************************************
*
* class: ScaleKeyFrame
*
* author: Mark Kellogg
*
* This class encapsulates a single scale key frame.
*
***********************************************/

#pragma once

#include "../Engine.h"
#include "../common/types.h"
#include "KeyFrame.h"
#include "../geometry/Vector3.h"
#include "../math/Matrix4x4.h"

namespace Core {

	class ScaleKeyFrame final: public KeyFrame {
	public:

		// scale transformation
		Vector3r Scale;

		ScaleKeyFrame();
		ScaleKeyFrame(Real normalizedTime, Real realTime, Real realTimeTicks, const Vector3r& scale);
		~ScaleKeyFrame() override;
	};

}
