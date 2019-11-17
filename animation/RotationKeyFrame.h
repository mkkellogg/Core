/*********************************************
*
* class: RotationKeyFrame
*
* author: Mark Kellogg
*
* This class encapsulates a single rotation key frame.
*
***********************************************/

#pragma once

#include "../Engine.h"
#include "../common/types.h"
#include "KeyFrame.h"
#include "../math/Quaternion.h"
#include "../math/Matrix4x4.h"

namespace Core {

	class RotationKeyFrame : public KeyFrame {
	public:

		// The rotation transformation
		Quaternion Rotation;

		RotationKeyFrame();
		RotationKeyFrame(Real normalizedTime, Real realTime, Real realTimeTicks, const Quaternion& rotation);
		virtual ~RotationKeyFrame() override;
	};
}
