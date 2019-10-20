#include "../Engine.h"
#include "AnimationPlayer.h"
#include "Bone.h"
#include "../base/CoreObject.h"
#include "../geometry/Vector3.h"
#include "../math/Quaternion.h"
#include "../math/Matrix4x4.h"
#include "../scene/Transform.h"
#include "../render/MeshContainer.h"
#include "Skeleton.h"
#include "AnimationInstance.h"
#include "Animation.h"
#include "AnimationManager.h"
#include "CrossFadeBlendOp.h"
#include "BlendOp.h"
#include "../common/types.h"
#include "../common/debug.h"
#include "../common/Constants.h"
#include "../util/Time.h"

namespace Core {

	/*
	* Single constructor, which initializes member variables.
	*/
	AnimationPlayer::AnimationPlayer(WeakPointer<Skeleton> target) {
		if (!target.isValid()) {
			throw InvalidReferenceException("AnimationPlayer::AnimationPlayer -> Invalid target.");
		}
		this->target = target;
		this->animationCount = 0;
		this->playingAnimationsCount = 0;
	}

	/*
	 * Destructor
	 */
	AnimationPlayer::~AnimationPlayer() {
	}

	/*
	 * Add a blending operation to the queue of active blending operations.
	 */
	void AnimationPlayer::queueBlendOperation(BlendOp * op) {
		if (op == nullptr) {
			throw NullPointerException("AnimationPlayer::queueBlendOperation -> 'op' is null.");
		}
		this->activeBlendOperations.push(op);
	}

	/*
	 * Get the currently active blend operation, if there is one. If there isn't one,
	 * return null;
	 */
	BlendOp * AnimationPlayer::getCurrentBlendOp() {
		if (this->activeBlendOperations.size() > 0) return this->activeBlendOperations.front();
		return nullptr;
	}

	/*
	 * Remove all blending operations from the blending operations queue [activeBlendOperations]
	 */
	void AnimationPlayer::clearBlendOpQueue() {
		UInt32 clearCount = 0;
		while (this->activeBlendOperations.size() > 0) {
			BlendOp * op = this->activeBlendOperations.front();
			if (clearCount == 0) op->onStoppedEarly();
			this->activeBlendOperations.pop();
			delete op;
			clearCount++;
		}
	}

	/*
	 * Update & drive the blending operation that is at the head of the queue of
	 * active blending operations.
	 */
	void AnimationPlayer::updateBlendingOperations() {
		// check if there are any active blending operations
		if (this->activeBlendOperations.size() > 0) {
			// retrieve (but don't remove) blending operation at head of queue
			BlendOp * op = this->activeBlendOperations.front();
			if (op == nullptr) {
				throw Exception("AnimationPlayer::updateBlending -> Null operation found in queue.");
			}

			// trigger OnStart callback for blending operation, if appropriate
			if (!op->hasStarted()) {
				op->setStarted(true);
				op->onStart();
			}

			// update blending operation
			op->update(animationWeights);

			// trigger OnComplete callback and deallocate blending operation, if appropriate
			if (op->hasCompleted()) {
				op->onComplete();
				this->activeBlendOperations.pop();
				delete op;
			}
		}
	}

	/*
	 * Add up weights of active animations, and calculate unused weight [leftOverWeight]
	 */
	void AnimationPlayer::checkWeights() {
		Real leftOverWeight = 1;
		for (UInt32 i = 0; i < this->registeredAnimations.size(); i++) {
			WeakPointer<AnimationInstance> instance = this->registeredAnimations[i];

			if (instance.isValid() && instance->playing) {
				leftOverWeight -= this->animationWeights[i];
			}
		}
		if (leftOverWeight < 0) leftOverWeight = 0;
	}

	/*
	 * Trigger all update sub-operations.
	 */
	void AnimationPlayer::update() {
		// update current blending operation
		this->updateBlendingOperations();
		// validate animation weights
		this->checkWeights();
		// update the positions of all nodes in the target skeleton based on
		// active animations
		this->applyActiveAnimations();
		// drive the progress of active animations
		this->updateAnimationsProgress();
	}

	/*
	 * Update the positions of all nodes of the target Skeleton object based on the progress of all
	 * active animations.
	 *
	 * This method loops through each node in the target skeleton [target], and for each node it calculates
	 * the interpolated translation, rotation, and scale for that node for each active animation. It combines
	 * those transformations based on the weight of each active animation stored in member [weights] and
	 * applies the final transformation to the node.
	 */
	void AnimationPlayer::applyActiveAnimations() {
		Vector3r translation;
		Vector3r scale;
		Quaternion rotation;

		// storage for aggregate translation, rotation, and scale
		Vector3r agTranslation;
		Vector3r agScale;
		Quaternion agRotation;

		Quaternion temp;

		// temp use Matrices
		Matrix4x4 rotMatrix;
		Matrix4x4 matrix;

		// keep track of the number of playing animations seen as we loop through all registered animations
		UInt32 playingAnimationsSeen = 0;

		// loop through each node in the target Skeleton object, and calculate the position based on
		// weighted average of positions returned from each active animation
		for (UInt32 node = 0; node < target->getNodeCount(); node++) {
			agScale.set(0, 0, 0);
			agTranslation.set(0, 0, 0);
			agRotation = Quaternion::Identity;

			playingAnimationsSeen = 0;
			Real agWeight = 0;

			// get the Skeleton node corresponding to the current node index
			Skeleton::SkeletonNode * targetNode = target->getNodeFromList(node);

			// loop through all registered animations
			for (Int32 i = (Int32)registeredAnimations.size() - 1; i >= 0; i--) {
				WeakPointer<AnimationInstance> instance = this->registeredAnimations[i];

				// include this animation only if it is playing
				if (instance.isValid() && instance->playing) {

					// if this node does not have an animation channel for it in the current animation, then ignore
					Int32 mappedChannel = instance->getChannelMappingForTargetNode(node);

					// retrieve this animation's weight
					Real weight = this->animationWeights[i];

					// if this animation's weight is 0, then ignore it
					if (weight <= 0)continue;

					// calculate aggregate (sum of weights up until this point)
					agWeight += weight;

					// calculate the translation, rotation, and scale for this animation at the current node
					if (mappedChannel >= 0) {
						this->calculateInterpolatedValues(instance, mappedChannel, translation, rotation, scale);
					}
					// if there is no channel in the current animation for this node, use the
					// default transformation values for this node
					else {
						translation = targetNode->InitialTranslation;
						rotation = targetNode->InitialRotation;
						scale = targetNode->InitialScale;
					}

					// if the number of active animations is 1, indicated by playingAnimationsCount == 1, and its
					// weight is 1, then we simply use the results from CalculateInterpolatedValues() and apply those
					// to the current node
					if (this->playingAnimationsCount == 1 && weight == 1) {
						agTranslation = translation;
						agScale = scale;
						agRotation = rotation;
					}
					// we have to apply weights across 1 or more animations
					else {

						// apply weight to each transformation
						translation.scale(weight);
						scale.scale(weight);
						rotation.set(rotation.x() * weight, rotation.y() * weight, rotation.z() * weight, rotation.w() * weight);

						// if this is the first active animation encountered, set the aggregate translation, rotation and scale
						if (playingAnimationsSeen == 0) {
							agTranslation = translation;
							agScale = scale;
							agRotation = rotation;
						}
						// this is not the first active animation encountered, so we additively combine the translation
						// and scale into their aggregate counterparts. we use spherical interpolation to combine the
						// rotation for this animation into the aggregate rotation.
						else {
							agTranslation = translation + agTranslation;
							agScale = scale + agScale;

							if (agWeight != 0)temp = Quaternion::slerp(agRotation, rotation, weight / agWeight);
							else temp = rotation;
							agRotation = temp;
						}
					}

					playingAnimationsSeen++;
				}
			}

			// only apply transformations if they were actually calculated
			if (playingAnimationsSeen > 0) {
				matrix.setIdentity();
				// apply interpolated scale
				matrix.scale(agScale.x, agScale.y, agScale.z);
				// apply interpolated rotation
				agRotation.normalize();
				rotMatrix = agRotation.rotationMatrix();
				matrix.preMultiply(rotMatrix);
				// apply interpolated translation
				matrix.preTranslate(agTranslation.x, agTranslation.y, agTranslation.z);

				// if the agWeight for some reason is less than one, compensate by using
				// the initial transformation values for the node
				if (agWeight < .99) {
					Matrix4x4 temp = targetNode->InitialTransform;
					temp.multiplyByScalar(((Real)1.0 - agWeight));
					matrix.add(temp);
				}

				if (targetNode->hasTarget()) {
					// get the local transform of the target of this node and apply
					// [matrix], which contains the interpolated scale, rotation, and translation
					Transform * localTransform = targetNode->getLocalTransform();
					if (localTransform != nullptr)localTransform->setLocalMatrix(matrix);
				}
			}
		}
	}

	/*
	 * Use the current progress of [instance] to find the two closest key frames in the KeyFrameSet specified by [channel].
	 * Then interpolate between those two key frames based on where the progress of [instance] lies between them, and store the
	 * interpolated translation, rotation, and scale values in [translation], [rotation], and [scale].
	 */
	void AnimationPlayer::calculateInterpolatedValues(WeakPointer<AnimationInstance> instance, UInt32 channel, Vector3r& translation, Quaternion& rotation, Vector3r& scale) const
	{
		Animation * animationPtr = const_cast<Animation *>(instance->sourceAnimation.get());
		KeyFrameSet * frameSet = animationPtr->getKeyFrameSet(channel);
		if (frameSet == nullptr) {
			throw NullPointerException("AnimationPlayer::calculateInterpolatedValues -> 'frameSet' is null.");
		}

		// make sure it's an active KeyFrameSet
		if (frameSet != nullptr && frameSet->Used) {
			// for each of translation, scale, and rotation, find the two respective key frames between which
			// instance->Progress lies, and interpolate between them based on instance->Progress.
			this->calculateInterpolatedTranslation(instance, *frameSet, translation);
			this->calculateInterpolatedScale(instance, *frameSet, scale);
			this->calculateInterpolatedRotation(instance, *frameSet, rotation);
		}
	}

	/*
	 * Drive the progress of each active animation.
	 */
	void AnimationPlayer::updateAnimationsProgress() {

		// loop through each registered animation and check if it is active. if it is,
		// call UpdateAnimationInstanceProgress() and pass [instance] to it.
		for (UInt32 i = 0; i < this->registeredAnimations.size(); i++) {
			WeakPointer<AnimationInstance> instance = this->registeredAnimations[i];
			if (instance.isValid() && instance->playing) {
				this->updateAnimationInstanceProgress(instance);
			}
		}
	}

	/*
	 * Drive the progress of [instance].
	 */
	void AnimationPlayer::updateAnimationInstanceProgress(WeakPointer<AnimationInstance> instance) const {
		// make sure the animation is active
		if (instance->playing && !instance->paused) {
			// update animation instance progress
			instance->progress += Time::getDeltaTime() * instance->speedFactor;

			Real effectiveEnd = (instance->duration > instance->earlyEnd) ? instance->earlyEnd : instance->duration;
			Real effectiveStart = (instance->startOffset > 0) ? instance->startOffset : 0;

			// has the animation reached the end?
			if (instance->progress > effectiveEnd)
			{
				if (instance->playBackMode == PlaybackMode::Repeat)
				{
					instance->progress = instance->progress - effectiveEnd + effectiveStart;
					if (instance->progress < effectiveStart) instance->progress = effectiveStart;
				}
				else if (instance->playBackMode == PlaybackMode::Clamp)
				{
					instance->progress = effectiveEnd;
				}
				else
				{
					instance->progress = effectiveEnd;
				}
			}

			instance->progressTicks = instance->progress * instance->sourceAnimation->getTicksPerSecond();
		}
	}

	/*
	 * Use the value of instance->progress to find the two closest translation key frames in [keyFrameSet]. Then interpolate between the translation
	 * values in those two key frames based on where instance->progress lies between them, and store the result in [vector].
	 */
	void AnimationPlayer::calculateInterpolatedTranslation(WeakPointer<AnimationInstance> instance, const KeyFrameSet& keyFrameSet, Vector3r& vector) const {
		if (!instance.isValid()) {
			throw InvalidReferenceException("AnimationPlayer::calculateInterpolatedTranslation -> 'instance' is invalid.");
		}

		UInt32 frameCount = (UInt32)keyFrameSet.TranslationKeyFrames.size();
		if (frameCount == 0) {
			throw Exception("AnimationPlayer::calculateInterpolatedTranslation -> Key frame count is zero.");
		}

		UInt32 previousIndex, nextIndex;
		Real interFrameProgress;
		Bool foundFrames = this->calculateInterpolation(instance, keyFrameSet, previousIndex, nextIndex, interFrameProgress, TransformationCompnent::Translation);

		// did we successfully find 2 frames between which to interpolate?
		if (foundFrames) {
			const TranslationKeyFrame& nextFrame = keyFrameSet.TranslationKeyFrames[nextIndex];
			const TranslationKeyFrame& previousFrame = keyFrameSet.TranslationKeyFrames[previousIndex];

			vector.x = ((nextFrame.Translation.x - previousFrame.Translation.x) * interFrameProgress) + previousFrame.Translation.x;
			vector.y = ((nextFrame.Translation.y - previousFrame.Translation.y) * interFrameProgress) + previousFrame.Translation.y;
			vector.z = ((nextFrame.Translation.z - previousFrame.Translation.z) * interFrameProgress) + previousFrame.Translation.z;

		} else { //we did not find 2 frames, so set translation equal to the first frame
			const TranslationKeyFrame& firstFrame = keyFrameSet.TranslationKeyFrames[0];
			vector.set(firstFrame.Translation.x, firstFrame.Translation.y, firstFrame.Translation.y);
		}
	}

	/*
	 * Use the value of instance->progress to find the two closest scale key frames in [keyFrameSet]. Then interpolate between the scale
	 * values in those two key frames based on where instance->progress lies between them, and store the result in [vector].
	 */
	void AnimationPlayer::calculateInterpolatedScale(WeakPointer<AnimationInstance> instance, const KeyFrameSet& keyFrameSet, Vector3r& vector) const {
		
		if (!instance.isValid()) {
			throw InvalidReferenceException("AnimationPlayer::calculateInterpolatedScale -> 'instance' is invalid.");
		}

		UInt32 frameCount = (UInt32)keyFrameSet.ScaleKeyFrames.size();
		if (frameCount == 0) {
			throw Exception("AnimationPlayer::calculateInterpolatedScale -> Key frame count is zero.");
		}

		UInt32 previousIndex, nextIndex;
		Real interFrameProgress;
		Bool foundFrames = calculateInterpolation(instance, keyFrameSet, previousIndex, nextIndex, interFrameProgress, TransformationCompnent::Scale);

		// did we successfully find 2 frames between which to interpolate?
		if (foundFrames) {
			const ScaleKeyFrame& nextFrame = keyFrameSet.ScaleKeyFrames[nextIndex];
			const ScaleKeyFrame& previousFrame = keyFrameSet.ScaleKeyFrames[previousIndex];

			// perform interpolation
			vector.x = ((nextFrame.Scale.x - previousFrame.Scale.x) * interFrameProgress) + previousFrame.Scale.x;
			vector.y = ((nextFrame.Scale.y - previousFrame.Scale.y) * interFrameProgress) + previousFrame.Scale.y;
			vector.z = ((nextFrame.Scale.z - previousFrame.Scale.z) * interFrameProgress) + previousFrame.Scale.z;
		} else {//we did not find 2 frames, so set scale equal to the first frame
			const ScaleKeyFrame& firstFrame = keyFrameSet.ScaleKeyFrames[0];
			vector.set(firstFrame.Scale.x, firstFrame.Scale.y, firstFrame.Scale.y);
		}
	}

	/*
	 * Use the value of instance->progress to find the two closest rotation key frames in [keyFrameSet]. Then interpolate between the rotation
	 * values in those two key frames based on where instance->progress lies between them, and store the result in [rotation].
	 */
	void AnimationPlayer::calculateInterpolatedRotation(WeakPointer<AnimationInstance> instance, const KeyFrameSet& keyFrameSet, Quaternion& rotation) const {
		
		if (!instance.isValid()) {
			throw InvalidReferenceException("AnimationPlayer::calculateInterpolatedRotation -> 'instance' is invalid.");
		}

		UInt32 frameCount = (UInt32)keyFrameSet.RotationKeyFrames.size();
		if (frameCount == 0) {
			throw Exception("AnimationPlayer::calculateInterpolatedRotation -> Key frame count is zero.");
		}

		UInt32 previousIndex, nextIndex;
		Real interFrameProgress;
		Bool foundFrames = this->calculateInterpolation(instance, keyFrameSet, previousIndex, nextIndex, interFrameProgress, TransformationCompnent::Rotation);

		// did we successfully find 2 frames between which to interpolate?
		if (foundFrames) {
			const RotationKeyFrame& nextFrame = keyFrameSet.RotationKeyFrames[nextIndex];
			const RotationKeyFrame& previousFrame = keyFrameSet.RotationKeyFrames[previousIndex];

			// perform spherical interpolation between the two Quaternions
			Quaternion a(previousFrame.Rotation.x(), previousFrame.Rotation.y(), previousFrame.Rotation.z(), previousFrame.Rotation.w());
			Quaternion b(nextFrame.Rotation.x(), nextFrame.Rotation.y(), nextFrame.Rotation.z(), nextFrame.Rotation.w());
			Quaternion quatOut = Quaternion::slerp(a, b, interFrameProgress);
			rotation.set(quatOut.x(), quatOut.y(), quatOut.z(), quatOut.w());
		} else {//we did not find 2 frames, so set rotation equal to the first frame
			const RotationKeyFrame& firstFrame = keyFrameSet.RotationKeyFrames[0];
			rotation.set(firstFrame.Rotation.x(), firstFrame.Rotation.y(), firstFrame.Rotation.z(), firstFrame.Rotation.w());
		}
	}

	/*
	 * This method uses the value of instance->progress to find the two closest key frames in [keyFrameSet], of the type specified by [component]
	 * and then stores the indices of those key frames in [previousIndex] and [nextIndex]. Then it uses instance->progress to determine how far from [lastIndex]
	 * to [nextIndex] the animation currently is, and stores that value in [interFrameProgress] (range: 0 to 1).
	 */
	Bool AnimationPlayer::calculateInterpolation(WeakPointer<AnimationInstance> instance, const KeyFrameSet& keyFrameSet, UInt32& previousIndex, UInt32& nextIndex, Real& interFrameProgress, TransformationCompnent component) const {
		if (!instance.isValid()) {
			throw InvalidReferenceException("AnimationPlayer::calculateInterpolation -> 'instance' is invalid.");
		}

		UInt32 frameCount = 0;
		Real progress = instance->progress;
		Real duration = instance->duration;

		// get the correct frame count, which depends on [component]
		if (component == TransformationCompnent::Translation)frameCount = (UInt32)keyFrameSet.TranslationKeyFrames.size();
		else if (component == TransformationCompnent::Rotation)frameCount = (UInt32)keyFrameSet.RotationKeyFrames.size();
		else if (component == TransformationCompnent::Scale)frameCount = (UInt32)keyFrameSet.ScaleKeyFrames.size();
		else return false;

		// loop through each key frame
		for (UInt32 f = 0; f < frameCount; f++) {

			KeyFrame * previousFrame = nullptr;
			KeyFrame * nextFrame = nullptr;

			// get the correct time stamp for this frame, which depends on [component]
			Real keyRealTime = this->getKeyFrameTime(component, f, keyFrameSet);

			// if the RealTime value for this key frame is greater than [progress], then the previous key frame and the current key frame
			// are the frames we want
			if (keyRealTime > progress || f == frameCount - 1) {
				previousIndex = 0;
				if (f > 0)previousIndex = f - 1;
				nextIndex = f;

				// flag that indicates we need to interpolate from the last frame to the first frame
				Bool overShoot = false;

				// if f==frameCount-1 and keyRealTime <= progress, then we have reached the last frame and progress has moved
				// beyond it. this means we need to interpolate between the last frame and the first frame (for smoothed animation looping).
				if (f == frameCount - 1 && keyRealTime <= progress)
				{
					previousIndex = f;
					nextIndex = 0;

					// if the start offset for this animation is > 0, then we can't assume the
					// next frame will be at index 0. in this case we must loop through each
					// frame to find which one has a timestamp greater than StartOffset.
					if (instance->startOffset > 0) {
						for (UInt32 ff = 0; ff < frameCount; ff++) {
							// get the correct time stamp for for frame [ff], which depends on [component]
							Real nextKeyRealTime = this->getKeyFrameTime(component, ff, keyFrameSet);
							if (nextKeyRealTime > instance->startOffset || ff == frameCount - 1) {
								nextIndex = ff;
							}
						}
					}
					overShoot = true;
				}

				// get pointers to the previous frame and the next frame
				if (component == TransformationCompnent::Translation) {
					const TranslationKeyFrame& nextTranslationFrame = keyFrameSet.TranslationKeyFrames[nextIndex];
					const TranslationKeyFrame& lastTranslationFrame = keyFrameSet.TranslationKeyFrames[previousIndex];
					nextFrame = const_cast<TranslationKeyFrame *>(&nextTranslationFrame);
					previousFrame = const_cast<TranslationKeyFrame *>(&lastTranslationFrame);
				} else if (component == TransformationCompnent::Rotation) {
					const RotationKeyFrame& nextRotationFrame = keyFrameSet.RotationKeyFrames[nextIndex];
					const RotationKeyFrame& lastRotationFrame = keyFrameSet.RotationKeyFrames[previousIndex];
					nextFrame = const_cast<RotationKeyFrame *>(&nextRotationFrame);
					previousFrame = const_cast<RotationKeyFrame *>(&lastRotationFrame);
				} else if (component == TransformationCompnent::Scale) {
					const ScaleKeyFrame& nextScaleFrame = keyFrameSet.ScaleKeyFrames[nextIndex];
					const ScaleKeyFrame& lastScaleFrame = keyFrameSet.ScaleKeyFrames[previousIndex];
					nextFrame = const_cast<ScaleKeyFrame *>(&nextScaleFrame);
					previousFrame = const_cast<ScaleKeyFrame *>(&lastScaleFrame);
				}

				// calculate local progress between [previous] and [nextFrame]
				Real interFrameTimeDelta = nextFrame->RealTime - previousFrame->RealTime;
				if (overShoot)  interFrameTimeDelta = duration - previousFrame->RealTime;

				Real interFrameElapsed = progress - previousFrame->RealTime;
				interFrameProgress = 1;
				if (interFrameTimeDelta > 0)interFrameProgress = interFrameElapsed / interFrameTimeDelta;

				return true;
			}
		}

		return false;
	}

	/*
	 * Get the key frame time for the frame at [frameIndex] for the desired transformation component [transformationComponent].
	 */
	Real AnimationPlayer::getKeyFrameTime(TransformationCompnent transformationComponent, Int32 frameIndex, const KeyFrameSet& keyFrameSet) const {
		Real keyFrameTime = 0;
		if (transformationComponent == TransformationCompnent::Translation)
			keyFrameTime = keyFrameSet.TranslationKeyFrames[frameIndex].RealTime;
		else if (transformationComponent == TransformationCompnent::Rotation)
			keyFrameTime = keyFrameSet.RotationKeyFrames[frameIndex].RealTime;
		else if (transformationComponent == TransformationCompnent::Scale)
			keyFrameTime = keyFrameSet.ScaleKeyFrames[frameIndex].RealTime;

		return keyFrameTime;
	}

	/*
	 * Add an Animation object [animation] to this player. The Animation must be compatible with
	 * the existing target of this player.
	 */
	void AnimationPlayer::addAnimation(WeakPointer<Animation> animation) {
		
		if(!animation.isValid()) {
			throw InvalidReferenceException("AnimationPlayer::createAnimationInstance -> 'animation' is invalid.");
		}

		WeakPointer<AnimationManager> animationManager = Engine::instance()->getAnimationManager();

		// verify compatibility with [target]
		if(!animationManager->isCompatible(this->target, animation)) {
			throw Exception("AnimationPlayer::createAnimationInstance -> Skeleton is not compatible with animation.");
		}

		// make sure an instance of [animation] does not already exist for this player
		if (this->animationIndexMap.find(animation->getObjectID()) == this->animationIndexMap.end()) {
			WeakPointer<AnimationInstance> instance = animationManager->createAnimationInstance(target, animation);
		
			Bool initSuccess = instance->init();
			if (!initSuccess) {
				throw Exception("AnimationPlayer::createAnimationInstance -> Unable to initialize animation instance.");
			}

			this->registeredAnimations.push_back(instance);
			this->animationWeights.push_back(0);
			this->crossFadeTargets.push_back(false);

			this->animationIndexMap[animation->getObjectID()] = animationCount;
			this->animationCount++;
		}
	}

	/*
	 * Set the normalized speed of playback of [animation] to [speedFactor].
	 */
	void AnimationPlayer::setSpeed(WeakPointer<Animation> animation, Real speedFactor) {

		if(!animation.isValid()) {
			throw InvalidReferenceException("AnimationPlayer::setSpeed -> 'animation' is invalid.");
		}

		if (this->animationIndexMap.find(animation->getObjectID()) != this->animationIndexMap.end()) {
			UInt32 targetIndex = this->animationIndexMap[animation->getObjectID()];
			this->setSpeed(targetIndex, speedFactor);
		}
	}

	void AnimationPlayer::setSpeed(UInt32 animationIndex, Real speedFactor) {

		if (animationIndex >= animationCount) {
			throw OutOfRangeException("AnimationPlayer::setSpeed -> 'animationIndex' is invalid.");
		}

		WeakPointer<AnimationInstance> instance = this->registeredAnimations[animationIndex];
		if (!instance.isValid()) {
			throw InvalidReferenceException("AnimationPlayer::SetSpeed -> Target animation is invalid.");
		}

		instance->speedFactor = speedFactor;
	}

	/*
	 * Start or resume playback of [animation] on the target of this player.
	 */
	void AnimationPlayer::play(WeakPointer<Animation> animation) {

		if(!animation.isValid()) {
			throw InvalidReferenceException("AnimationPlayer::play -> 'animation' is invalid.");
		}

		if (animationIndexMap.find(animation->getObjectID()) != this->animationIndexMap.end()) {
			UInt32 targetIndex = this->animationIndexMap[animation->getObjectID()];
			this->play(targetIndex);
		}
	}

	/*
	 * Start or resume playback of registered animation at [animationIndex].
	 */
	void AnimationPlayer::play(UInt32 animationIndex) {
		
		if (animationIndex >= animationCount) {
			throw OutOfRangeException("AnimationPlayer::play -> 'animationIndex' is invalid.");
		}

		Real tempLeftOver = 1;
		for (UInt32 i = 0; i < registeredAnimations.size(); i++) {

			WeakPointer<AnimationInstance> instance = this->registeredAnimations[i];

			if (i != animationIndex) {
				instance->stop();
				animationWeights[i] = 0;
			} else {
				animationWeights[i] = 1;
				instance->play();
			}

			tempLeftOver -= animationWeights[1];
		}
		if (tempLeftOver < 0)tempLeftOver = 0;
		this->playingAnimationsCount = 1;
	}

	/*
	 * Stop playback of [animation] on this player.
	 */
	void AnimationPlayer::stop(WeakPointer<Animation> animation) {

		if(!animation.isValid()) {
			throw InvalidReferenceException("AnimationPlayer::stop -> 'animation' is invalid.");
		}

		if (this->animationIndexMap.find(animation->getObjectID()) != this->animationIndexMap.end()) {
			UInt32 targetIndex = this->animationIndexMap[animation->getObjectID()];
			if (targetIndex >= animationCount) {
				throw OutOfRangeException("AnimationPlayer::Stop -> invalid animation index found in index map.");
			}
			this->stop(targetIndex);
		}
	}

	/*
	 * Stop playback of registered animation at [animationIndex].
	 */
	void AnimationPlayer::stop(UInt32 animationIndex) {

		if (animationIndex >= animationCount) {
			throw OutOfRangeException("AnimationPlayer::stop -> 'animationIndex' is invalid.");
		}

		WeakPointer<AnimationInstance> instance = this->registeredAnimations[animationIndex];
		if (!instance.isValid()) {
			throw InvalidReferenceException("AnimationPlayer::stop -> Target animation is invalid.");
		}

		if (instance->playing) this->playingAnimationsCount--;
		instance->stop();

		this->animationWeights[animationIndex] = 0;
	}

	/*
	 * Pause playback of [animation] on this player.
	 */
	void AnimationPlayer::pause(WeakPointer<Animation> animation) {
		
		if(!animation.isValid()) {
			throw InvalidReferenceException("AnimationPlayer::pause -> 'animation' is invalid.");
		}

		if (this->animationIndexMap.find(animation->getObjectID()) != this->animationIndexMap.end()) {
			UInt32 targetIndex = this->animationIndexMap[animation->getObjectID()];
			if (targetIndex >= animationCount) {
				throw OutOfRangeException("AnimationPlayer::pause -> invalid animation index found in index map.");
			}
			this->pause(targetIndex);
		}
	}

	/*
	 * Pause playback of registered animation at [animationIndex].
	 */
	void AnimationPlayer::pause(UInt32 animationIndex) {

		if (animationIndex >= animationCount) {
			throw OutOfRangeException("AnimationPlayer::pause -> 'animationIndex' is invalid.");
		}

		WeakPointer<AnimationInstance> instance = this->registeredAnimations[animationIndex];
		if (!instance.isValid()) {
			throw InvalidReferenceException("AnimationPlayer::pause -> Target animation is invalid.");
		}

		instance->pause();
	}

	/*
	 * Resume playback of [animation] on this player.
	 */
	void AnimationPlayer::resume(WeakPointer<Animation> animation) {
		
		if(!animation.isValid()) {
			throw InvalidReferenceException("AnimationPlayer::resume -> 'animation' is invalid.");
		}

		if (this->animationIndexMap.find(animation->getObjectID()) != this->animationIndexMap.end()) {
			UInt32 targetIndex = this->animationIndexMap[animation->getObjectID()];
			if (targetIndex >= animationCount) {
				throw OutOfRangeException("AnimationPlayer::resume -> invalid animation index found in index map.");
			}

			WeakPointer<AnimationInstance> instance = this->registeredAnimations[targetIndex];
			if (!instance.isValid()) {
				throw InvalidReferenceException("AnimationPlayer::resume -> Target animation is invalid.");
			}

			instance->play();
		}
	}

	/*
	 * Resume playback of registered animation at [animationIndex].
	 */
	void AnimationPlayer::resume(UInt32 animationIndex) {
		if (animationIndex >= this->animationCount) {
			throw OutOfRangeException("AnimationPlayer::resume -> 'animationIndex' is invalid.");
		}

		WeakPointer<AnimationInstance> instance = this->registeredAnimations[animationIndex];
		if (!instance.isValid()) {
			throw InvalidReferenceException("AnimationPlayer::resume -> Target animation is invalid.");
		}

		instance->play();
	}

	/*
	 * Cross-fade any currently playing animations over to the target animation [target].
	 */
	void AnimationPlayer::crossFade(WeakPointer<Animation> target, Real duration) {
		this->crossFade(target, duration, false);
	}

	/*
	 * Cross-fade any currently playing animations over to the target animation [target].
	 *
	 * This method initiates the cross-fade process, which means that all actively playing
	 * animations that are not [target] will be faded/blended out over [duration] seconds.
	 * At the same time the target animation will be faded/blended in at the same rate,
	 * creating a smooth transition to [target].
	 */
	void AnimationPlayer::crossFade(WeakPointer<Animation> target, Real duration, Bool queued) {
		if (this->animationIndexMap.find(target->getObjectID()) != this->animationIndexMap.end()) {
			UInt32 targetIndex = animationIndexMap[target->getObjectID()];
			if (targetIndex >= animationCount) {
				throw OutOfRangeException("AnimationPlayer::crossFade -> invalid animation index found in index map.");
			}

			WeakPointer<AnimationInstance> instance = this->registeredAnimations[targetIndex];
			if (!instance.isValid()) {
				throw InvalidReferenceException("AnimationPlayer::crossFade -> Target animation is invalid.");
			}

			// if a crossfade operation is currently active with the same target, then do nothing
			if (crossFadeTargets[targetIndex] == 1)return;

			CrossFadeBlendOp * blendOp = new(std::nothrow) CrossFadeBlendOp(duration, targetIndex);
			if (blendOp == nullptr) {
				throw AllocationException("AnimationPlayer::crossFade -> Unable to allocate new CrossFadeBlendOp object.");
			}

			Bool initSuccess = blendOp->init(this->animationWeights);
			if (!initSuccess) {
				throw Exception("AnimationPlayer::crossFade -> Unable to init new CrossFadeBlendOp object.");
			}

			crossFadeTargets[targetIndex] = 1;
			blendOp->setOnStartCallback([targetIndex, this](CrossFadeBlendOp * op) {
				WeakPointer<AnimationInstance> targetInstance = this->registeredAnimations[targetIndex];
				if (!targetInstance.isValid()) {
					throw InvalidReferenceException("AnimationPlayer::crossFade::setOnStartCallback -> Invalid target animation.");
				}

				Bool initSuccess = op->init(animationWeights);
				if (!initSuccess) {
					throw Exception("AnimationPlayer::crossFade::setOnStartCallback -> Unable to init CrossFadeBlendOp object.");
				}

				if (!targetInstance->playing) {
					this->playingAnimationsCount++;
					targetInstance->reset();
					targetInstance->play();
				}
			});

			blendOp->setOnStoppedEarlyCallback([targetIndex, this](CrossFadeBlendOp * op) {
				this->crossFadeTargets[targetIndex] = 0;
			});

			blendOp->setOnCompleteCallback([targetIndex, this](CrossFadeBlendOp * op) {
				for (UInt32 i = 0; i < this->registeredAnimations.size(); i++) {
					if (i != targetIndex) {
						WeakPointer<AnimationInstance> instance = this->registeredAnimations[i];
						if (!instance.isValid()) {
							throw InvalidReferenceException("AnimationPlayer::crossFade::onCompleteCallback -> Invalid animation found.");
						}
						this->stop(i);
						this->animationWeights[i] = 0;
					} else {
						animationWeights[i] = 1;
					}
				}
				this->crossFadeTargets[targetIndex] = 0;
				this->playingAnimationsCount = 1;
			});

			// If [queued] == false, then we want to start the cross-fade immediately. This
			// means we need to clear the
			if (!queued) this->clearBlendOpQueue();

			this->queueBlendOperation(blendOp);
		}
	}


	void AnimationPlayer::setPlaybackMode(WeakPointer<Animation> target, PlaybackMode playbackMode) {
		if (this->animationIndexMap.find(target->getObjectID()) != this->animationIndexMap.end()) {
			UInt32 targetIndex = this->animationIndexMap[target->getObjectID()];
			if (targetIndex >= animationCount) {
				throw OutOfRangeException( "AnimationPlayer::setPlaybackMode -> invalid animation index found in index map.");
			}

			WeakPointer<AnimationInstance> instance = this->registeredAnimations[targetIndex];
			if (!instance.isValid()) {
				throw InvalidReferenceException("AnimationPlayer::setPlaybackMode -> Target animation is invalid.");
			}

			instance->playBackMode = playbackMode;
		}
	}
}
