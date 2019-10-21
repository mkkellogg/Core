
#include "MeshContainer.h"
#include "../animation/VertexBoneMap.h"

namespace Core {

    void MeshContainer::setSkeleton(WeakPointer<Skeleton> skeleton) {
        this->skeleton = skeleton;
    }

    WeakPointer<Skeleton> MeshContainer::getSkeleton() {
        return this->skeleton;
    }

    void MeshContainer::addVertexBoneMap(UInt32 meshIndex, WeakPointer<VertexBoneMap> vertexBoneMap) {
        this->vertexBoneMaps[meshIndex] = vertexBoneMap;
    }

}