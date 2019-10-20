#pragma once

#include <memory>
#include <unordered_map>

#include "../common/types.h"
#include "../geometry/Mesh.h"
#include "RenderableContainer.h"

namespace Core {

    // forward declarations
    class VertexBoneMap;

    class MeshContainer : public RenderableContainer<Mesh> {

    public:

        void addVertexBoneMap(UInt32 meshIndex, WeakPointer<VertexBoneMap> vertexBoneMap);

    private:

        std::unordered_map<UInt32, PersistentWeakPointer<VertexBoneMap>> vertexBoneMaps;
    };

}
