/*********************************************
*
* class: TranslationKeyFrame
*
* author: Mark Kellogg
*
* This class encapsulates a single translation key frame.
*
***********************************************/

#pragma once

#include "../Engine.h"
#include "../common/types.h"
#include "KeyFrame.h"
#include "../geometry/Vector3.h"
#include "../math/Matrix4x4.h"

namespace Core
{
	class TranslationKeyFrame final: public KeyFrame {
	public:

		// translation transformation
		Vector3r Translation;

		TranslationKeyFrame();
		TranslationKeyFrame(Real normalizedTime, Real realTime, Real realTimeTicks, const Vector3r& translation);
		~TranslationKeyFrame() override;
	};
}
