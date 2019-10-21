/*********************************************
*
* class: Animation
*
* author: Mark Kellogg
*
* This class encapsulates key frame sequences for all nodes
* in a Skeleton object.
*
***********************************************/

#pragma once

#include <string>

#include "../Engine.h"
#include "../base/CoreObject.h"
#include "KeyFrameSet.h"
#include "../geometry/Vector3.h"
#include "../math/Quaternion.h"
#include "../math/Matrix4x4.h"
#include "../common/types.h"

#include <vector>

namespace Core {

	class Animation : public CoreObject {

		friend class Engine;
		friend class AnimationManager;
		friend class ModelLoader;

	public:

		~Animation();
		
		void clipEnds(Real startOffsetTicks, Real earlyEndTicks);
		UInt32 getChannelCount() const;
		KeyFrameSet * getKeyFrameSet(UInt32 nodeIndex);
		const std::string * getChannelName(UInt32 index) const;
		void setChannelName(UInt32 index, const std::string& name);
		Real getTicksPerSecond() const;
		Real getDurationTicks() const;
		Real getStartOffsetTicks() const;
		Real getEarlyEndTicks() const;
		Real getDuration() const;
		Real getStartOffset() const;
		Real getEarlyEnd() const;

	private:

		// A KeyFrameSet for each node in the target skeleton
		KeyFrameSet * keyFrames;

		// the channel name of each key frame set, used in animation targeting
		// 1:1 correspondence with [keyFrames]
		std::string * channelNames;

		// store the number of KeyFrameSet objects that have been allocated, which
		// is also the length of [channelNames]
		UInt32 channelCount;

		// the duration of this animation in  device/clock independent ticks
		Real durationTicks;
		// map the ticks duration to actual time
		Real ticksPerSecond;

		// we can have the animation start with progress > 0
		Real startOffsetTicks;

		// we can have the animation end earlier than [durationTicks]
		Real earlyEndTicks;

		Animation(Real durationTicks, Real ticksPerSecond);
		Animation(Real durationTicks, Real ticksPerSecond, Real startOffsetTicks, Real earlyEndTicks);

		void destroy();
		Bool init(UInt32 channelCount);
	};
}
