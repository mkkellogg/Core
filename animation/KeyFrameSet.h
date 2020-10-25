/*********************************************
*
* class: KeyFrameSet
*
* author: Mark Kellogg
*
* This class encapsulates all the key frames in an animation for a single node/transform.
*
***********************************************/

#pragma once

#include <vector>

#include "../Engine.h"
#include "TranslationKeyFrame.h"
#include "ScaleKeyFrame.h"
#include "RotationKeyFrame.h"
#include "../common/types.h"

namespace Core {
	
	class KeyFrameSet final {
	public:

		// is this key frame set active?
		Bool Used;
		// key frames with translation transformations
		std::vector<TranslationKeyFrame> TranslationKeyFrames;
		// key frames with scale transformations
		std::vector<ScaleKeyFrame> ScaleKeyFrames;
		// key frames with rotation transformations
		std::vector<RotationKeyFrame> RotationKeyFrames;

		KeyFrameSet();
		~KeyFrameSet();
	};
}
