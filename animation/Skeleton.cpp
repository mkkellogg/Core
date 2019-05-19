#include "Skeleton.h"
#include "Bone.h"
#include "../util/Tree.h"

namespace Core {
    /*
    * Only constructor.
    */
    Skeleton::Skeleton(UInt32 boneCount) {
        this->boneCount = boneCount;
        this->bones = nullptr;
    }
    /*
     * Destructor.
     */
    Skeleton::~Skeleton() {
        this->destroy();
    }

    /*
     * Destroy the skeleton. Delete all Bone objects, and
     * all SkeletonNode objects.
     */
    void Skeleton::destroy() {
        // delete all Bone objects
        if (this->bones != nullptr) {
            delete[] this->bones;
            this->bones = nullptr;
        }
        this->boneNameMap.clear();

        // delete all SkeletonNode objects by traversing the node hierarchy and
        // using a visitor to invoke the callback below, which performsm the delete.
        this->skeleton.setTraversalCallback([](Tree<Skeleton::SkeletonNode *>::TreeNode * node) -> Bool {
            if (node != nullptr && node->Data != nullptr) {
                delete node->Data;
                node->Data = nullptr;
            }
            return true;
        });

        this->skeleton.traverse();
    }
    /*
     * Get the number of bones in this skeleton.
     */
    UInt32 Skeleton::getBoneCount() const {
        return this->boneCount;
    }

    /*
     * Get the number of nodes in this skeleton.
     */
    UInt32 Skeleton::getNodeCount() const {
        return (UInt32)this->nodeList.size();
    }

    /*
     * Initialize the Skeleton object. This MUST be called before the Skeleton object can
     * be used in any way. It will return true if initialization success; false otherwise.
     */
    Bool Skeleton::init() {
        // destroy existing data (if there is any)
        this->destroy();

        this->bones = new(std::nothrow) Bone[boneCount];
        if (this->bones == nullptr) {
            throw AllocationException("Skeleton::init -> Could not allocate bone array.");
        }
        
        return true;
    }

    /*
     * Create a root node for this Skeleton object. The root node is a dummy node and only serves
     * to connect all other nodes to a single root ancestor. It does not have a bone associated with
     * it and does not have a target. It has no transformation associated with it.
     */
    Tree<Skeleton::SkeletonNode*>::TreeNode * Skeleton::createRoot(Skeleton::SkeletonNode * node) {
        if (this->skeleton.getRoot() == nullptr) {
            this->skeleton.addRoot(node);
        }
        return this->skeleton.getRoot();
    }

    /*
     * Connect a child node [child] to a parent node [parent].
     */
    Tree<Skeleton::SkeletonNode*>::TreeNode * Skeleton::addChild(Tree<Skeleton::SkeletonNode*>::TreeNode * parent, Skeleton::SkeletonNode * node) {
        ASSERT(parent != nullptr, "Skeleton::addChild -> 'parent' is null.");
        Tree<Skeleton::SkeletonNode*>::TreeNode * childNode = parent->addChild(node);
        return childNode;
    }

    /*
     * Set the mapping from a bone name to its index in [boneNameMap]
     */
    void Skeleton::mapBone(const std::string& name, UInt32 boneIndex) {
        this->boneNameMap[name] = boneIndex;
    }

    /*
     * Get the index in [boneNameMap] to which [name] corresponds. If no valid mapping
     * is found, -1 is returned.
     */
    Int32 Skeleton::getBoneMapping(const std::string& name) const {
        std::unordered_map<std::string, UInt32>::const_iterator result =this-> boneNameMap.find(name);
        if (result != this->boneNameMap.end()) {
            return (*result).second;
        }
        return -1;
    }

    /*
     * Get the Bone object stored at [index] in [bones].
     */
    Bone* Skeleton::getBone(UInt32 boneIndex) {
        ASSERT(boneIndex < boneCount, "Skeleton::getBone -> 'boneIndex' is out of range.");
        return this->bones + boneIndex;
    }

    /*
     * Set the mapping from a node name to its index in [nodeNameMap]
     */
    void Skeleton::mapNode(std::string& name, UInt32 nodeIndex) {
        this->nodeNameMap[name] = nodeIndex;
    }

    /*
     * Get the index in [nodeName] to which [name] corresponds. If no valid mapping
     * is found, -1 is returned.
     */
    Int32 Skeleton::getNodeMapping(const std::string& name) const {
        std::unordered_map<std::string, UInt32>::const_iterator result = this->nodeNameMap.find(name);
        if (result != this->nodeNameMap.end()) {
            return (*result).second;
        }

        return -1;
    }

    /*
     * Get the SkeletonNode object stored at [index] in [nodeList].
     */
    Skeleton::SkeletonNode * Skeleton::getNodeFromList(UInt32 nodeIndex) {
        if (nodeIndex >= this->getNodeCount()) {
            Debug::PrintError("Skeleton::GetNodeFromList -> Index is out of range.");
            return nullptr;
        }

        return nodeList[nodeIndex];
    }

    /*
     * Add SkeletonNode object to [nodeList].
     */
    void Skeleton::addNodeToList(Skeleton::SkeletonNode * node) {
       this->nodeList.push_back(node);
    }

    /*
     * Replace the bones in this skeleton with matching bones from [skeleton].
     */
    void Skeleton::overrideBonesFrom(WeakPointer<const Skeleton> skeleton, Bool takeOffset, Bool takeNode) {
        ASSERT(skeleton.isValid(), "Skeleton::overrideBonesFrom -> 'skeleton' is not valid.");
        this->overrideBonesFrom(skeleton.get(), takeOffset, takeNode);
    }

    /*
     * Replace the bones in this skeleton with matching bones from [skeleton].
     */
    void Skeleton::overrideBonesFrom(const Skeleton * skeleton, Bool takeOffset, Bool takeNode) {
        for (UInt32 n = 0; n < skeleton->getBoneCount(); n++) {
            Bone * newBone = const_cast<Skeleton *>(skeleton)->getBone(n);
            for (UInt32 c = 0; c < this->getBoneCount(); c++) {
                Bone * currentBone = this->getBone(c);
                if (currentBone != nullptr && newBone != nullptr && newBone->Name == currentBone->Name) {
                    if (takeOffset) currentBone->OffsetMatrix = newBone->OffsetMatrix;
                    if (takeNode) currentBone->Node = newBone->Node;
                    break;
                }
            }
        }
    }

    /*
     * Create a full (deep) clone of this Skeleton object.
     */
    Skeleton * Skeleton::fullClone() {
        // allocate new Skeleton object
        Skeleton * newSkeleton = new(std::nothrow) Skeleton(boneCount);
        if (newSkeleton == nullptr) {
            throw AllocationException("Skeleton::fullClone -> could not allocate skeleton.");
        }
        // initialize new skeleton
        Bool initSuccess = newSkeleton->init();
        if (!initSuccess) {
            delete newSkeleton;
            return nullptr;
        }

        // copy over all the bones from this skeleton
        for (UInt32 i = 0; i < boneCount; i++) {
            newSkeleton->getBone(i)->copy(this->getBone(i));
        }

        // copy over the bone name map
        newSkeleton->boneNameMap = this->boneNameMap;

        // get the TreeNode object that contains the root node of the existing skeleton
        Tree<Skeleton::SkeletonNode *>::TreeNode * rootTreeNode = this->skeleton.getRoot();

        if (rootTreeNode != nullptr) {
            Skeleton::SkeletonNode * rootClone = nullptr;

            // create a clone of the root SkeletonNode
            if (rootTreeNode->Data != nullptr) {
                rootClone = rootTreeNode->Data->fullClone();
                ASSERT(rootClone != nullptr, "Skeleton::fullClone -> Could not clone root node.");
            }

            // create new root TreeNode and set its Data to the new root SkeletonNode
            Tree<Skeleton::SkeletonNode *>::TreeNode * newRoot = newSkeleton->createRoot(rootClone);
            ASSERT(newRoot != nullptr, "Skeleton::fullClone -> Could not create new root node.");

            // map from TreeNode objects in the existing skeleton to their counterparts in the new skeleton
            std::unordered_map<Tree<Skeleton::SkeletonNode *>::TreeNode *, Tree<Skeleton::SkeletonNode *>::TreeNode *> newNodeMap;
            newNodeMap[rootTreeNode] = newRoot;

            Bool allocateTreeSuccess = true;

            // Create a TreeNode visitor callback that will be used to visit each node in the existing skeleton in order to:
            // (1) create a copy of that TreeNode
            // (2) map the existing TreeNode to the corresponding TreeNode in the new skeleton.
            // After the existing skeleton tree is traversed using this visitor callback, the tree is not fully cloned. At that
            // point we will only have copies of the TreeNode objects in the skeleton tree, and not the SkeletonNode objects
            // that TreeNode::Data should point to. Additionally none of those TreeNode copies will have their children or parents
            // set yet, so there will be no hierarchy information available.
            skeleton.setTraversalCallback([&allocateTreeSuccess, &newNodeMap](Tree<Skeleton::SkeletonNode *>::TreeNode * node) -> Bool {
                Tree<Skeleton::SkeletonNode *>::TreeNode * newNode = new(std::nothrow) Tree<Skeleton::SkeletonNode *>::TreeNode();
                ASSERT(newNode != nullptr, "Skeleton::fullClone -> Could not allocte new node.");
                newNodeMap[node] = newNode;
                return true;
            });
            skeleton.traverse();

            ASSERT(allocateTreeSuccess, "Skeleton::fullClone -> Could not allocate new nodes for skeleton clone");

            // copy over the node name map
            newSkeleton->nodeNameMap = nodeNameMap;

            // pre-allocate nodeList space
            newSkeleton->nodeList.resize(nodeList.size());

            Bool cloneTreeSuccess = true;
            Skeleton * thisSkeleton = this;

            // Create a second TreeNode visitor callback for the existing skeleton. The purpose of this callback is to:
            // (1) create a clone of the Data field of each TreeNode that is visited, which is the SkeletonNode object, and attach to
            //     the corresponding cloned node in the new skeleton.
            // (2) attach the cloned TreeNode in the new skeleton to its appropriate parent TreeNode in the new skeleton.
            skeleton.setTraversalCallback([&cloneTreeSuccess, &newNodeMap, newSkeleton, thisSkeleton](Tree<Skeleton::SkeletonNode *>::TreeNode * node) -> Bool {
                Skeleton::SkeletonNode * clonedSkeletonNode = nullptr;
                if (node != nullptr && node->Data != nullptr) {
                    // clone the SkeletonNode
                    clonedSkeletonNode = node->Data->fullClone();
                    ASSERT(clonedSkeletonNode != nullptr, "Skeleton::fullClone -> Could not clone node in skeletal tree.");
                }

                // find the corresponding TreeNode in the new skeleton and attach the cloned SkeletonNode
                Tree<Skeleton::SkeletonNode *>::TreeNode * clonedTreeNode = newNodeMap[node];
                clonedTreeNode->Data = clonedSkeletonNode;

                // enter the cloned SkeletonNode in the [nodeList] of the new Skeleton object.
                Int32 targetListIndex = thisSkeleton->getNodeMapping(clonedSkeletonNode->Name);
                if (targetListIndex >= 0) newSkeleton->nodeList[targetListIndex] = clonedSkeletonNode;

                // the cloned TreeNode will still have a pointer to its original parent in the existing skeleton
                Tree<Skeleton::SkeletonNode *>::TreeNode * existingParent = clonedTreeNode->getParent();

                // find counterpart of existing parent in the new skeleton, and add the cloned TreeNode as a child
                if (existingParent != nullptr) {
                    Tree<Skeleton::SkeletonNode *>::TreeNode * clonedParentNode = newNodeMap[existingParent];
                    clonedParentNode->addChild(clonedTreeNode);
                }

                // find the Bone object in the new skeleton that corresponds to the cloned SkeletonNode,
                // and set its SkeletonNode pointer [Node] to the clone SkeletonNode object.
                Int32 boneIndex = node->Data->BoneIndex;
                if (boneIndex >= 0) {
                    newSkeleton->getBone(boneIndex)->Node = clonedSkeletonNode;
                }

                return true;
            });
            skeleton.traverse();

            // check for successful clone
            ASSERT(cloneTreeSuccess, "Skeleton::fullClone -> Could not clone skeletal tree.");
        }

        return newSkeleton;
    }
}
