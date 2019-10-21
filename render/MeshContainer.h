#pragma once

#include <memory>
#include <unordered_map>

#include "../common/types.h"
#include "../geometry/Mesh.h"
#include "../animation/Skeleton.h"
#include "RenderableContainer.h"

namespace Core {

    // forward declarations
    class VertexBoneMap;

    class MeshContainer : public RenderableContainer<Mesh> {

    public:

        void setSkeleton(WeakPointer<Skeleton> skeleton);
        WeakPointer<Skeleton> getSkeleton();
        void addVertexBoneMap(UInt32 meshIndex, WeakPointer<VertexBoneMap> vertexBoneMap);

    private:

        PersistentWeakPointer<Skeleton> skeleton;
        std::unordered_map<UInt32, PersistentWeakPointer<VertexBoneMap>> vertexBoneMaps;
    };

}
