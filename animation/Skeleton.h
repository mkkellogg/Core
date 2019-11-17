/*********************************************
*
* class: Skeleton
*
* author: Mark Kellogg
*
* A Skeleton encapsulates a hierarchy/tree of transformations, each of which is encapsulated
* by a SkeletonNode object. These nodes don't necessarily contain the transformation information
* themselves (although they might via polymorphism), and may instead point to other objects
* that contain actual transformation information.
*
* A Skeleton object also contains bones (of course). Bones contain additional information beyond
* hierarchy transformation information, such as an offset transformation which moves
* from model space into bone space, and is used when performing vertex skinning. All bones have
* a single corresponding node in the skeleton, but a node does not have to have a bone.
*
***********************************************/

#pragma once

#include <functional>
#include <vector>
#include <string>
#include <unordered_map>
#include <memory>

#include "../common/types.h"
#include "../scene/Object3D.h"
#include "../base/CoreObject.h"
#include "../geometry/Vector3.h"
#include "../math/Matrix4x4.h"
#include "../math/Quaternion.h"
#include "../util/Tree.h"

namespace Core {

    //forward declarations
    class Bone;
    class Transform;

    class Skeleton : public CoreObject {

        friend class Engine;

    public:

        class SkeletonNode {
        public:

            // the index of the corresponding bone (if there is one) in the container Skeleton object's
            // bone array.
            Int32 BoneIndex;
            // the name of this node
            std::string Name;
            // save the original transformations
            Matrix4x4 InitialTransform;
            Vector3r InitialTranslation;
            Vector3r InitialScale;
            Quaternion InitialRotation;

            SkeletonNode(Int32 boneIndex, const std::string& name) {
                this->BoneIndex = boneIndex;
                this->Name = name;
            }
            virtual ~SkeletonNode() {}

            virtual Matrix4x4& getFullTransform() = 0;
            virtual Matrix4x4& getLocalTransform() = 0;
            virtual Bool hasTarget() const = 0;
            virtual SkeletonNode * fullClone() const = 0;
        };

        virtual ~Skeleton();
        UInt32 getBoneCount() const;
        UInt32 getNodeCount() const;

        Bool init();
        Tree<SkeletonNode*>::TreeNode * createRoot(SkeletonNode* node);
        Tree<SkeletonNode*>::TreeNode * addChild(Tree<SkeletonNode*>::TreeNode * parent, SkeletonNode* node);

        void mapBone(const std::string& name, UInt32 boneIndex);
        Int32 getBoneMapping(const std::string& name) const;
        Bone* getBone(UInt32 boneIndex);

        void mapNode(std::string& name, UInt32 nodeIndex);
        Int32 getNodeMapping(const std::string& name) const;
        SkeletonNode * getNodeFromList(UInt32 nodeIndex);
        Tree<SkeletonNode*>::TreeNode * getRootNode();
        void addNodeToList(SkeletonNode * node);

        void overrideBonesFrom(WeakPointer<const Skeleton> skeleton, Bool takeOffset, Bool takeNode);
        void overrideBonesFrom(const Skeleton * skeleton, Bool takeOffset, Bool takeNode);
    
    private:
        
        // number of bones in this skeleton
        UInt32 boneCount;
        // indexed list of all the bones in this skeleton
        Bone * bones;
        // map from bone name to index in [bones] for the matching Bone object
        std::unordered_map<std::string, UInt32> boneNameMap;

        // map from node name to index in [nodeList] for the mtching SkeletonNode object
        std::unordered_map<std::string, UInt32> nodeNameMap;

        // indexed list of all the nodes in this skeleton
        std::vector<SkeletonNode *> nodeList;

        // contains transformation hierarchy structure
        Tree<SkeletonNode*> skeleton;

        Skeleton(UInt32 boneCount);

        void destroy();
        Skeleton * fullClone();
    };
}
