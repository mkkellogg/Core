#include "../Engine.h"
#include "Animation.h"
#include "Skeleton.h"
#include "../common/types.h"

namespace Core {

	/*
	 * Simple constructor, calls the main constructor with [startOffsetTicks] = 0 && [earlyEndTicks] = [durationTicks]
	 */
	Animation::Animation(Real durationTicks, Real ticksPerSecond) : Animation(durationTicks, ticksPerSecond, 0, durationTicks) {
	}

	/*
	 * Main constructor - initializes all member variables of this animation.
	 */
	Animation::Animation(Real durationTicks, Real ticksPerSecond, Real startOffsetTicks, Real earlyEndTicks) {
		//force ticksPerSecond > 0
		if (ticksPerSecond <= 0)ticksPerSecond = 1;

		keyFrames = nullptr;
		this->durationTicks = durationTicks;
		this->ticksPerSecond = ticksPerSecond;
		this->startOffsetTicks = startOffsetTicks;
		this->earlyEndTicks = earlyEndTicks;
		channelCount = 0;
		channelNames = nullptr;

		if (this->durationTicks < 0)this->durationTicks = 0;
		if (this->startOffsetTicks < 0)this->startOffsetTicks = 0;
		if (this->earlyEndTicks < 0)this->earlyEndTicks = 0;

		if (this->earlyEndTicks > this->durationTicks)this->earlyEndTicks = this->durationTicks;
		if (this->startOffsetTicks > this->earlyEndTicks)this->startOffsetTicks = this->earlyEndTicks;
	}

	/*
	 * Cleanup the animation
	 */
	Animation::~Animation() {
		destroy();
	}

	/*
	 * This method destroys [channelNames] and [keyFrames] and invalidates their pointers
	 */
	void Animation::destroy() {
		if (keyFrames != nullptr) {
			delete[] keyFrames;
			keyFrames = nullptr;
		}

		if (channelNames != nullptr) {
			delete[] channelNames;
			channelNames = nullptr;
		}

		channelCount = 0;
	}

	/*
	 * Initialize this animation. This method will allocate [keyFrameSetCount]] key frame sets.
	 */
	Bool Animation::init(UInt32 channelCount) {
		destroy();

		this->channelCount = channelCount;

		keyFrames = new(std::nothrow) KeyFrameSet[channelCount];
		if (keyFrames == nullptr) {
			throw AllocationException("Animation::init -> Could not allocate key frame set array.");
		}

		channelNames = new(std::nothrow) std::string[channelCount];
		if (channelNames == nullptr) {
			throw AllocationException("Animation::init -> Could not allocate channel name set array.");
		}

		return true;
	}

	/*
	 * Clip the beginning [startOffset] seconds from the animation, and the end
	 * [earlyEnd] seconds from the animation.
	 */
	void Animation::clipEnds(Real startOffset, Real earlyEnd) {
		this->startOffsetTicks = startOffset * ticksPerSecond;
		this->earlyEndTicks = earlyEnd * ticksPerSecond;

		if (this->earlyEndTicks < 0) this->earlyEndTicks = 0;
		if (this->startOffsetTicks < 0) this->startOffsetTicks = 0;
	}

	/*
	 * Return the number of KeyFrameSet objects in [keyFrames].
	 */
	UInt32 Animation::getChannelCount() const {
		return channelCount;
	}

	/*
	 * Retrieve the KeyFrameSet corresponding to a particular node in [target].
	 */
	KeyFrameSet * Animation::getKeyFrameSet(UInt32 nodeIndex) {
		if (nodeIndex >= channelCount) {
			throw OutOfRangeException("Animation::getKeyFrameSet -> Node index is out of range.");
		}
		return keyFrames + nodeIndex;
	}

	const std::string * Animation::getChannelName(UInt32 index) const {
		if (index >= channelCount) {
			throw OutOfRangeException("Animation::getChannelName -> 'index' is out of range.");
		}
		return channelNames + index;
	}

	void Animation::setChannelName(UInt32 index, const std::string& name) {
		if (index >= channelCount) {
			throw OutOfRangeException("Animation::setChannelName -> 'index' is out of range.");
		}
		channelNames[index] = name;
	}

	/*
	 * Get the duration of this animation in ticks.
	 */
	Real Animation::getDurationTicks() const {
		return durationTicks;
	}

	/*
	 * Get the mapping of duration ticks to seconds.
	 */
	Real Animation::getTicksPerSecond() const {
		return ticksPerSecond;
	}

	/*
	 * Get the start offset of this animation in ticks.
	 */
	Real Animation::getStartOffsetTicks() const {
		return startOffsetTicks;
	}

	/*
	 * Get the point where this animation ends relative to its full duration in ticks.
	 */
	Real Animation::getEarlyEndTicks() const {
		return earlyEndTicks;
	}

	/*
	 * Get the duration of this animation in seconds.
	 */
	Real Animation::getDuration() const {
		return durationTicks / ticksPerSecond;
	}

	/*
	 * Get the start offset of this animation in seconds.
	 */
	Real Animation::getStartOffset() const {
		return startOffsetTicks / ticksPerSecond;
	}

	/*
	 * Get the point where this animation ends relative to its full duration in seconds.
	 */
	Real Animation::getEarlyEnd() const {
		return earlyEndTicks / ticksPerSecond;
	}
}
