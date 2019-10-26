
#include "MeshContainer.h"
#include "../animation/VertexBoneMap.h"

namespace Core {

    void MeshContainer::setSkeleton(WeakPointer<Skeleton> skeleton) {
        this->skeleton = skeleton;
    }

    WeakPointer<Skeleton> MeshContainer::getSkeleton() {
        return this->skeleton;
    }

    void MeshContainer::addVertexBoneMap(UInt64 meshID, WeakPointer<VertexBoneMap> vertexBoneMap) {
        this->vertexBoneMaps[meshID] = vertexBoneMap;
        this->vertexBoneMapSet[meshID] = true;
    }

    WeakPointer<VertexBoneMap> MeshContainer::getVertexBoneMap(UInt64 meshID) {
        if (this->vertexBoneMaps.find(meshID) != this->vertexBoneMaps.end()) {
            return this->vertexBoneMaps[meshID];
        } else {
            throw InvalidArgumentException("MeshContainer::getVertexBoneMap() -> Invalid mesh id.");
        }
    }

    Bool MeshContainer::hasVertexBoneMap(UInt64 meshID) {
        return this->vertexBoneMapSet[meshID];
    }

}