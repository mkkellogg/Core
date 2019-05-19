/*********************************************
*
* class: Object3DSkeletonNode
*
* author: Mark Kellogg
*
* This class inherits from SkeletonNode and provides functionality
* for a node that specifically targets Object3D.
*
***********************************************/

#pragma once

#include <string>

#include "Skeleton.h"
#include "../common/types.h"

namespace Core {

    //forward declarations
    class Transform;
    class Object3D;

    class Object3DSkeletonNode final : public Skeleton::SkeletonNode {
    public:

        // target Object3D
        WeakPointer<Object3D> Target;

        Object3DSkeletonNode(WeakPointer<Object3D> target, Int32 boneIndex, const std::string& name);
        ~Object3DSkeletonNode();

        const Transform * getFullTransform() const override;
        Transform * getLocalTransform() override;
        Bool hasTarget() const override;
        SkeletonNode * fullClone() const override;
    };
}
