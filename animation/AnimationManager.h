/*********************************************
*
* class: AnimationManager
*
* author: Mark Kellogg
*
* This class manages and drives all instances of AnimationPlayer, and
* therefore, all playing animations.
*
***********************************************/

#pragma once

#include <unordered_map>

#include "../Engine.h"
#include "../common/types.h"
#include "../base/CoreObject.h"

namespace Core {

	// forward declarations
	class AnimationPlayer;
	class AnimationInstance;
	class Animation;
	class Skeleton;

	class AnimationManager {

		// necessary to trigger lifecycle events and manage allocation
		friend class Engine;

	public:

		~AnimationManager();
		Bool isCompatible(WeakPointer<Skeleton> skeleton, WeakPointer<Animation> animation) const;
		void update();
		WeakPointer<AnimationPlayer> retrieveOrCreateAnimationPlayer(WeakPointer<Skeleton> target);
		WeakPointer<AnimationInstance> createAnimationInstance(WeakPointer<Skeleton> target, WeakPointer<Animation> animation);

	private:

		AnimationManager();

		// map object IDs of Skeleton objects to their assign animation player
		std::unordered_map<UInt64, WeakPointer<AnimationPlayer>> activePlayers;
		std::vector<std::shared_ptr<AnimationPlayer>> allPlayers;
		std::vector<std::shared_ptr<AnimationInstance>> instances;
	};
}
