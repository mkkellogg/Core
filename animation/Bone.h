/*********************************************
*
* class: Bone
*
* author: Mark Kellogg
*
* This class represents a single bone in a Skeleton object.
*
***********************************************/

#pragma once

#include <string>

#include "../common/types.h"
#include "../Engine.h"
#include "../geometry/Vector3.h"
#include "../math/Matrix4x4.h"
#include "../math/Quaternion.h"

namespace Core {

    //forward declarations
    class Transform;
    //class SkeletonNode;

    class Bone {
    public:

        Bone();
        Bone(const std::string& name);
        Bone(const std::string& name, UInt32 id);
        virtual ~Bone();

        // the bone's (unique) name. Used for linking this Bone object
        // to its corresponding node in a Skeleton or scene object hierarchy.
        std::string Name;
        // this bone's unique ID
        UInt32 ID;
        // this matrix converts the bone (and attached vertices) to bone space
        Matrix4x4 OffsetMatrix;
        Matrix4x4 TempFullMatrix;
        // when this bone is part of a Skeleton object, [Node] points to this bone's
        // corresponding SkeletonNode object in that Skeleton object.
        void * Node;

        void copy(const Bone * bone);
    };
}
