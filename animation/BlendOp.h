/*********************************************
*
* class: BlendOp
*
* author: Mark Kellogg
*
* This class is the base class for animation blending operations.
*
***********************************************/

#pragma once

#include <vector>
#include <string>

#include "../Engine.h"
#include "../common/types.h"

namespace Core {

	//forward declarations
	class Transform;
	class SkeletonNode;

	class BlendOp {

		friend class AnimationPlayer;

	public:

		void reset();
		Bool init(const std::vector<Real>& initialWeights);
		virtual void update(std::vector<Real>& weights);
		virtual void onStart() = 0;
		virtual void onComplete() = 0;
		virtual void onStoppedEarly() = 0;
		Bool hasCompleted() const;
		Bool hasStarted() const;
		Real getNormalizedProgress() const;
		Real getProgress() const;

	protected:

		Real duration;
		Real progress;
		Bool complete;
		Bool started;

		std::vector<Real> initialWeights;

		BlendOp(Real duration);
		virtual ~BlendOp();

		void setComplete(Bool complete);
		void setStarted(Bool started);
	};
}
