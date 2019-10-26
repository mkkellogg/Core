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
        void addVertexBoneMap(UInt64 meshID, WeakPointer<VertexBoneMap> vertexBoneMap);
        WeakPointer<VertexBoneMap> getVertexBoneMap(UInt64 meshID);
        Bool hasVertexBoneMap(UInt64 meshID);

    private:

        PersistentWeakPointer<Skeleton> skeleton;
        std::unordered_map<UInt64, PersistentWeakPointer<VertexBoneMap>> vertexBoneMaps;
        std::unordered_map<UInt64, Bool> vertexBoneMapSet;
    };

}
