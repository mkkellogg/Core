#include "Object3DSkeletonNode.h"
#include "../scene/Object3D.h"

namespace Core {
    /*
    * Only constructor, which is parameterized.
    */
    Object3DSkeletonNode::Object3DSkeletonNode(WeakPointer<Object3D> target, Int32 boneIndex, const std::string& name) : Skeleton::SkeletonNode(boneIndex, name) {
        this->Target = target;
    }

    /*
     * Destructor.
     */
    Object3DSkeletonNode::~Object3DSkeletonNode() {}

    /*
     * This method gets the full transform of the target Object3D, meaning its local transform
     * concatenated with the transforms of all its ancestors.
     */
    const Transform * Object3DSkeletonNode::getFullTransform() const {
        throw Exception("Object3DSkeletonNode::getFullTransform -> revisit this!!");
        if (!this->Target.isValid()) {
            throw InvalidReferenceException("Object3DSkeletonNode::getFullTransform -> Node does not have a valid target.");
        }
        WeakPointer<Object3D> target = this->Target;
        return &target->getTransform();
    }

    /*
     * Retrieve only the local transform of the target Object3D.
     */
    Transform * Object3DSkeletonNode::getLocalTransform() {
        if (!this->Target.isValid()) {
            throw InvalidReferenceException("Object3DSkeletonNode::getLocalTransform -> Node does not have a valid target.");
        }
        Transform& ref = Target->getTransform();
        return &ref;
    }

    /*
     * Is this node pointed at a valid Object3D target?
     */
    Bool Object3DSkeletonNode::hasTarget() const {
        return Target.isValid();
    }

    /*
     * Create a full (deep) clone of this node.
     */
    Skeleton::SkeletonNode * Object3DSkeletonNode::fullClone() const {
        Skeleton::SkeletonNode * newNode = new(std::nothrow) Object3DSkeletonNode(this->Target, this->BoneIndex, this->Name);
        if (newNode == nullptr) {
            throw AllocationException("Object3DSkeletonNode::fullClone -> Could not allocate new node.");
        }
        newNode->InitialTransform = this->InitialTransform;
        newNode->InitialTranslation = this->InitialTranslation;
        newNode->InitialScale = this->InitialScale;
        newNode->InitialRotation = this->InitialRotation;

        return newNode;
    }
}
