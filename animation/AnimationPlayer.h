/*********************************************
*
* class: AnimationPlayer
*
* author: Mark Kellogg
*
* This class manages playback of all animations for a single Skeleton target.
*
***********************************************/

#pragma once

#include <vector>
#include <queue>
#include <unordered_map>
#include <string>

#include "../Engine.h"
#include "../base/CoreObject.h"
#include "../geometry/Vector3.h"
#include "../math/Quaternion.h"
#include "KeyFrameSet.h"

namespace Core {
	//forward declarations
	class Transform;
	class SkeletonNode;
	class BlendOp;
	class Animation;
	class AnimationInstance;

	enum class TransformationCompnent {
		Translation = 0,
		Rotation = 1,
		Scale = 2
	};

	enum class PlaybackMode {
		Repeat = 0,
		Clamp = 1,
		PingPong = 2
	};

	class AnimationPlayer final {

		friend class Engine;
		friend class AnimationManager;

	public:

		~AnimationPlayer();
		void addAnimation(WeakPointer<Animation> animation);
		void setSpeed(WeakPointer<Animation> animation, Real speedFactor);
		void play(WeakPointer<Animation> animation);
		void stop(WeakPointer<Animation> animation);
		void pause(WeakPointer<Animation> animation);
		void resume(WeakPointer<Animation> animation);
		void crossFade(WeakPointer<Animation> target, Real duration);
		void crossFade(WeakPointer<Animation> target, Real duration, Bool queued);
		void setPlaybackMode(WeakPointer<Animation> target, PlaybackMode playbackMode);

	private:

		// number of animations being handled by this player
		UInt32 animationCount;
		// mapping from object ID's of Animation objects to respective indices in member arrays/vectors
		std::unordered_map<UInt64, UInt32> animationIndexMap;
		// target of all animations managed by this player
		PersistentWeakPointer<Skeleton> target;
		// mapping from the object ID's of Animation objects to corresponding AnimationInstance objects
		std::vector<PersistentWeakPointer<AnimationInstance>> registeredAnimations;
		// weights used for animation blending
		std::vector<Real> animationWeights;
		// active animation blending operations
		std::queue<BlendOp*> activeBlendOperations;
		// flags that indicate if the animation at a specified index is the target of a cross fade operation in activeBlendOperations
		std::vector<Bool> crossFadeTargets;
		// number of animations currently playing
		Int32 playingAnimationsCount;

		AnimationPlayer(WeakPointer<Skeleton> target);

		void queueBlendOperation(BlendOp * op);
		BlendOp * getCurrentBlendOp();
		void clearBlendOpQueue();

		void update();
		void updateBlendingOperations();
		void checkWeights();
		void applyActiveAnimations();
		void updateAnimationsProgress();
		void updateAnimationInstanceProgress(WeakPointer<AnimationInstance> instance) const;
		void calculateInterpolatedValues(WeakPointer<AnimationInstance> instance, UInt32 channel, Vector3r& translation, Quaternion& rotation, Vector3r& scale) const;
		void calculateInterpolatedTranslation(WeakPointer<AnimationInstance> instance, const KeyFrameSet& keyFrameSet, Vector3r& vector) const;
		void calculateInterpolatedScale(WeakPointer<AnimationInstance> instance, const KeyFrameSet& keyFrameSet, Vector3r& vector) const;
		void calculateInterpolatedRotation(WeakPointer<AnimationInstance> instance, const KeyFrameSet& keyFrameSet, Quaternion& rotation) const;
		Bool calculateInterpolation(WeakPointer<AnimationInstance> instance, const KeyFrameSet& keyFrameSet, UInt32& lastIndex, UInt32& nextIndex, Real& interFrameProgress, TransformationCompnent component) const;
		Real getKeyFrameTime(TransformationCompnent transformationComponent, Int32 frameIndex, const KeyFrameSet& keyFrameSet) const;

		void setSpeed(UInt32 animationIndex, Real speedFactor);
		void play(UInt32 animationIndex);
		void stop(UInt32  animationIndex);
		void pause(UInt32  animationIndex);
		void resume(UInt32  animationIndex);
	};
}
