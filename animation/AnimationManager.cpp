#include <stdio.h>
#include <stdlib.h>

#include <string>
#include <unordered_map>

#include "../Engine.h"
#include "AnimationManager.h"
#include "../geometry/Vector3.h"
#include "../math/Quaternion.h"
#include "Animation.h"
#include "AnimationInstance.h"
#include "AnimationPlayer.h"
#include "Skeleton.h"
#include "Bone.h"
#include "../util/Time.h"

namespace Core {

	/*
	* Default constructor
	*/
	AnimationManager::AnimationManager() {
	}

	/*
	 * Destructor
	 */
	AnimationManager::~AnimationManager() {
	}

	/*
	 * Check if the Skeleton specified by [skeleton] is compatible with [animation]. This simply
	 * means loop through each channel in [animation] and verify there is a corresponding node in
	 * [skeleton. This match-up is accomplished by matching the name of the channel to the name
	 * of the skeleton node.
	 */
	Bool AnimationManager::isCompatible(WeakPointer<Skeleton> skeleton, WeakPointer<Animation> animation) const {

		if (!skeleton.isValid()) {
			throw InvalidReferenceException("AnimationManager::isCompatible -> Skeleton is not valid.");
		}

		if (!animation.isValid()) {
			throw InvalidReferenceException("AnimationManager::isCompatible -> Animation is not valid.");
		}

		UInt32 skeletonNodeCount = skeleton->getNodeCount();
		UInt32 channelCount = animation->getChannelCount();

		// verify each channel in the animation has a matching node in [skeleton]
		for (UInt32 c = 0; c < channelCount; c++) {
			Bool foundNodeForChannel = false;
			const std::string * channelName = animation->getChannelName(c);
			if (channelName == nullptr)continue;

			for (UInt32 n = 0; n < skeletonNodeCount; n++) {
				Skeleton * skeletonPtr = const_cast<Skeleton *>(skeleton.get());
				Skeleton::SkeletonNode * node = skeletonPtr->getNodeFromList(n);
				if (node->Name == *channelName) {
					foundNodeForChannel = true;
					break;
				}
			}

			if (!foundNodeForChannel) {
				//std::string msg("AnimationManager::IsCompatible -> Could not find matching node for: ");
				//msg += *channelName;
				//Debug::PrintWarning(msg);
				//return false;
			}
		}

		return true;
	}

	/*
	 * Loop through each active AnimationPlayer and drive its playback.
	 */
	void AnimationManager::update() {
		for (std::unordered_map<UInt64, WeakPointer<AnimationPlayer>>::iterator iter = this->activePlayers.begin(); iter != activePlayers.end(); ++iter) {
			WeakPointer<AnimationPlayer> player = iter->second;
			if (player.isValid()) {
				player->update();
			}
		}
	}

	/*
	 * Check active players to see if any are playing animations for [target]. If not, create one
	 * and assign it to [target].
	 */
	WeakPointer<AnimationPlayer> AnimationManager::retrieveOrCreateAnimationPlayer(WeakPointer<Skeleton> target) {
		if (!target.isValid()) {
			throw InvalidReferenceException("AnimationManager::retrieveOrCreateAnimationPlayer -> Target is not valid.");
		}
	
		if (activePlayers.find(target->getObjectID()) == activePlayers.end()) {
			AnimationPlayer * playerPtr = new(std::nothrow) AnimationPlayer(target);
			if (playerPtr == nullptr) {
				throw AllocationException( "AnimationManager::retrieveOrCreateAnimationPlayer -> Could not allocate new AnimationPlayer object.");
			}

			std::shared_ptr<AnimationPlayer> player(playerPtr);
			this->allPlayers.push_back(player);
			// put the newly created AnimationPlayer in the list of active players.s
			this->activePlayers[target->getObjectID()] = player;
			return player;
		}

		return activePlayers[target->getObjectID()];
	}

	WeakPointer<AnimationInstance> AnimationManager::createAnimationInstance(WeakPointer<Skeleton> target, WeakPointer<Animation> animation) {

		if (!target.isValid()) {
			throw InvalidReferenceException("AnimationManager::createAnimationInstance -> Target is not valid.");
		}

		if (!animation.isValid()) {
			throw InvalidReferenceException("AnimationManager::createAnimationInstance -> Animation is not valid.");
		}

        AnimationInstance * instancePtr = new(std::nothrow) AnimationInstance(target, animation);
        if (instancePtr == nullptr) {
			throw AllocationException("AnimationManager::createAnimationInstance -> Could not allocate new AnimationInstance object.");
		}

        std::shared_ptr<AnimationInstance> instance(instancePtr);
		this->instances.push_back(instance);
	}
}





