
#include "MeshContainer.h"
#include "../animation/VertexBoneMap.h"

namespace Core {

    void MeshContainer::addVertexBoneMap(UInt32 meshIndex, WeakPointer<VertexBoneMap> vertexBoneMap) {
        this->vertexBoneMaps[meshIndex] = vertexBoneMap;
    }

}