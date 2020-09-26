#pragma once

#include <unordered_map>
#include <vector>

#include "../common/types.h"
#include "../util/WeakPointer.h"
#include "RenderQueue.h"

namespace Core {

    // forward declarations
    class Object3D;
    class MeshRenderer;
    class Mesh;

    class RenderQueueManager {
    public:

        RenderQueueManager();

        UInt32 getRenderQueueCount() const;
        void clearAll();
        void addItemToQueue(UInt32 queueID, WeakPointer<BaseObject3DRenderer> renderer,
                            WeakPointer<BaseRenderable> renderable, Bool isStatic, Bool isActive);
        void addMeshToQueue(UInt32 queueID, WeakPointer<MeshRenderer> meshRenderer, WeakPointer<Mesh> mesh, Bool isStatic, Bool isActive);
        RenderQueue& getRenderQueue(UInt32 index);

    protected:
        RenderQueue& getOrAddRenderQueue(UInt32 queueID);
        std::unordered_map<UInt32, RenderQueue> queues;
        std::vector<RenderQueue*> orderedQueues;

    private:
    };

}