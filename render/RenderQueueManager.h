#pragma once

#include <unordered_map>
#include <vector>

#include "../common/types.h"
#include "../util/WeakPointer.h"
#include "RenderQueue.h"

namespace Core {

    // forward declarations
    class Object3D;

    class RenderQueueManager {
    public:

        RenderQueueManager();

        UInt32 getRenderQueueCount() const;
        void clearAll();
        void addItemToQueue(UInt32 queueID, WeakPointer<BaseObjectRenderer> objectRenderer);
        RenderQueue& getRenderQueue(UInt32 index);

    protected:
        std::unordered_map<UInt32, RenderQueue> queues;
        std::vector<RenderQueue*> orderedQueues;

    private:
    };

}