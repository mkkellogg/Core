/*********************************************
*
* class: CrossFadeBlendOp
*
* author: Mark Kellogg
*
* This class is the base class for animation blending operations.
*
***********************************************/

#pragma once

#include <vector>
#include <string>
#include <memory>

#include "../Engine.h"
#include "BlendOp.h"
#include "../common/types.h"

namespace Core {

	//forward declarations
	class Transform;
	class SkeletonNode;

	class CrossFadeBlendOp final: public BlendOp {

		// necessary since AnimationPlayer directly controls this class
		friend class AnimationPlayer;

		UInt32 targetIndex;
		std::function<void(CrossFadeBlendOp*)> startCallback;
		std::function<void(CrossFadeBlendOp*)> completeCallback;
		std::function<void(CrossFadeBlendOp*)> stoppedEarlyCallback;

		CrossFadeBlendOp(Real duration, UInt32 targetIndex);

	protected:

		~CrossFadeBlendOp() override;

	public:

		void update(std::vector<Real>& weights) override;
		void onStart() override;
		void onComplete() override;
		void onStoppedEarly() override;
		void setOnStartCallback(std::function<void(CrossFadeBlendOp*)> callback);
		void setOnCompleteCallback(std::function<void(CrossFadeBlendOp*)> callback);
		void setOnStoppedEarlyCallback(std::function<void(CrossFadeBlendOp*)> callback);
		UInt32 getTargetIndex() const;
	};
}
