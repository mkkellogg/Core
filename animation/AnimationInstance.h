/*********************************************
*
* class: AnimationInstance
*
* author: Mark Kellogg
*
* This class encapsulates the state of a single instance
* of the playback an Animation object.
*
***********************************************/

#pragma once

#include "../Engine.h"
#include "../common/types.h"
#include "AnimationPlayer.h"

namespace Core {

	// forward declarations
	class Animation;
	class Skeleton;

	class AnimationInstance {

		friend class Engine;
		friend class AnimationManager;
		friend class AnimationPlayer;

	public:

		/*
		 * The FrameState class is used to keep track of the state of the animation for
		 * each node in the target Skeleton [Target]. The Animation object has a member
		 * array [FrameStates] which holds the instance for each node.
		 */
		class FrameState {

			public:

			// index of the current translation key for the node
			UInt32 TranslationKeyIndex;
			// index of the current scale key for the node
			UInt32 ScaleKeyIndex;
			// index of the current rotation key for the node
			UInt32 RotationKeyIndex;

			FrameState() {
				TranslationKeyIndex = 0;
				ScaleKeyIndex = 0;
				RotationKeyIndex = 0;
			}

			void Reset() {
				TranslationKeyIndex = 0;
				ScaleKeyIndex = 0;
				RotationKeyIndex = 0;
			}

		};

		virtual ~AnimationInstance();

		Bool init();
		void reset();

		FrameState * getFrameState(UInt32 stateIndex);
		Int32 getChannelMappingForTargetNode(UInt32 nodeIndex) const;

	private:

		// map each node in [Target] to a channel in [SourceAnimation]
		Int32 * nodeToChannelMap;

		// the target of this instance
		PersistentWeakPointer<Skeleton> target;
		// the Animation for which this is an instance
		PersistentWeakPointer<Animation> sourceAnimation;
		// used to make animation play faster or slow than default. default = 1.
		Real speedFactor;

		// number of states in [FrameStates]. This number should equal the number of nodes in [target].
		// it should also be equal to the number of KeyFrameSet objects in [SourceAnimation].
		UInt32 stateCount;
		// array of FrameState objects, one for each node in [target] and is indexed in the same way.
		// E.g. The FrameState at index 5 corresponds to the SkeletonNode returned by target->GetNode(5);
		FrameState * frameStates;

		// duration of this instance in seconds
		Real duration;
		// current progress (in seconds) of this instance
		Real progress;

		// duration of this instance in ticks
		Real durationTicks;
		// current progress (in ticks) of this instance
		Real progressTicks;

		// we can have the animation start with progress > 0
		Real startOffset;
		// equivalent of [StartOffset] in ticks
		Real startOffsetTicks;

		// we can have the animation end earlier than [Duration]
		Real earlyEnd;
		// equivalent of [EarlyEnd] in ticks
		Real earlyEndTicks;

		// is the animation playing?
		Bool playing;

		// is the animation paused?
		Bool paused;

		// what should happen when playback reaches the end of the clip?
		PlaybackMode playBackMode;

		AnimationInstance(WeakPointer<Skeleton> target, WeakPointer<Animation> animation);

		void destroy();
		void setSpeed(Real speedFactor);
		void play();
		void stop();
		void pause();

	};

}
