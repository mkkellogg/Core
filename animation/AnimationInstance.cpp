#include "AnimationInstance.h"
#include "../render/MeshContainer.h"
#include "Animation.h"
#include "Skeleton.h"

namespace Core {

	/*
	* Single constructor that initializes member variables.
	*/
	AnimationInstance::AnimationInstance(WeakPointer<Skeleton> target, WeakPointer<Animation> animation) {
		this->target = target;
		this->sourceAnimation = animation;

		this->stateCount = 0;
		this->frameStates = nullptr;

		this->nodeToChannelMap = nullptr;

		this->duration = 0;
		this->progress = 0;
		this->speedFactor = 1;

		this->durationTicks = 0;
		this->progressTicks = 0;

		this->earlyEndTicks = 0;
		this->earlyEnd = 0;

		this->startOffsetTicks = 0;
		this->startOffset = 0;

		this->playing = false;
		this->paused = false;

		this->playBackMode = PlaybackMode::Repeat;
	}

	/*
	 * Default destructor
	 */
	AnimationInstance::~AnimationInstance() {
		this->destroy();
	}

	/*
	 * Destroy this animation instance. Delete all FrameState instances in [FrameStates].
	 */
	void AnimationInstance::destroy() {
		if (this->frameStates != nullptr) {
			delete[] this->frameStates;
			this->frameStates = nullptr;
		}

		if (this->nodeToChannelMap != nullptr) {
			delete[] this->nodeToChannelMap;
			this->nodeToChannelMap = nullptr;
		}

		this->stateCount = 0;
	}

	/*
	 * Initialize this instance. This method will allocate a FrameState object for each node in [Target].
	 * It will also verify the the number of nodes in [Target] matches the number of KeyFrameSet objects
	 * contained in [SourceAnimation].
	 */
	Bool AnimationInstance::init() {
		this->destroy();

		if (!this->target.isValid()) {
			throw InvalidReferenceException("AnimationInstance::init -> Animation target is invalid.");
		}

		if (!this->sourceAnimation.isValid()) {
			throw InvalidReferenceException("AnimationInstance::init -> Animation is invalid.");
		}

		UInt32 nodeCount = this->target->getNodeCount();
		UInt32 channelCount = this->sourceAnimation->getChannelCount();

		if (nodeCount <= 0)return true;

		this->frameStates = new(std::nothrow) FrameState[nodeCount];
		if (this->frameStates == nullptr) {
			throw AllocationException("AnimationInstance::init -> Unable to allocate FrameState array.");
		}

		this->nodeToChannelMap = new(std::nothrow) int[nodeCount];
		if (this->nodeToChannelMap == nullptr) {
			throw AllocationException("AnimationInstance::init -> Unable to allocate NodeToChannelMap array.");
		}

		this->stateCount = nodeCount;

		for (UInt32 n = 0; n < nodeCount; n++) {
			Skeleton::SkeletonNode * node = this->target->getNodeFromList(n);
			Int32 foundIndex = -1;

			if (node != nullptr) {
				for (UInt32 c = 0; c < channelCount; c++) {
					const std::string * channelName = this->sourceAnimation->getChannelName(c);
					if (channelName == nullptr) continue;

					if (node->Name == *channelName) {
						foundIndex = c;
					}
				}
			}

			this->nodeToChannelMap[n] = foundIndex;
		}

		this->earlyEndTicks = this->sourceAnimation->getEarlyEndTicks();
		this->earlyEnd = this->earlyEndTicks / this->sourceAnimation->getTicksPerSecond();

		this->startOffsetTicks = this->sourceAnimation->getStartOffsetTicks();
		this->startOffset = this->startOffsetTicks / this->sourceAnimation->getTicksPerSecond();

		this->durationTicks = this->sourceAnimation->getDurationTicks();
		this->duration = this->durationTicks / this->sourceAnimation->getTicksPerSecond();

		this->progress = this->startOffset;
		this->progressTicks = this->startOffsetTicks;

		return true;
	}

	/*
	 * Reset this instance by resetting each FrameState object for each node in [Target] as
	 * well as setting [Progress] and [ProgressTicks] to 0.
	 */
	void AnimationInstance::reset() {

		for (UInt32 s = 0; s < this->stateCount; s++) {
			this->frameStates[s].Reset();
		}

		this->progress = this->startOffset;
		this->progressTicks = this->startOffsetTicks;
	}

	/*
	 * Retrieve the FrameState for a single node in [Target] via its index.
	 */
	AnimationInstance::FrameState * AnimationInstance::getFrameState(UInt32 stateIndex) {
		if (stateIndex >= this->stateCount) {
			throw OutOfRangeException("AnimationInstance::getFrameState -> State index is out of bounds.");
		}

		return this->frameStates + stateIndex;
	}

	Int32 AnimationInstance::getChannelMappingForTargetNode(UInt32 nodeIndex) const {
		if (!this->target.isValid()) {
			throw InvalidReferenceException("AnimationInstance::getChannelMappingForTargetNode -> target skeleton is not valid.");
		}

		if (nodeIndex >= this->target->getNodeCount()) {
			throw InvalidReferenceException("AnimationInstance::getChannelMappingForTargetNode -> nodeIndex is out of range.");
		}

		return this->nodeToChannelMap[nodeIndex];
	}

	void AnimationInstance::setSpeed(Real speedFactor) {
		this->speedFactor = speedFactor;
	}

	void AnimationInstance::play() {
		this->playing = true;
		this->paused = false;
	}

	void AnimationInstance::stop() {
		this->playing = false;
		this->reset();
	}

	void AnimationInstance::pause() {
		this->paused = true;
	}
}
