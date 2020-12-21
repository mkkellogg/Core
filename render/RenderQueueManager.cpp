#include <algorithm>

#include "RenderQueueManager.h"

namespace Core {

    RenderQueueManager::RenderQueueManager() {

    }

    UInt32 RenderQueueManager::getRenderQueueCount() const {
        return this->orderedQueues.size();
    }

    void RenderQueueManager::clearAll() {
        for (std::vector<RenderQueue*>::iterator it = this->orderedQueues.begin(); it != this->orderedQueues.end(); ++it) {
            RenderQueue* renderQueue = *it;
            renderQueue->clear();
        }
    }

    void RenderQueueManager::addItemToQueue(UInt32 queueID, WeakPointer<BaseObject3DRenderer> renderer,
                                            WeakPointer<BaseRenderable> renderable, Bool isStatic, Bool isActive, Int32 layer) {
        RenderQueue& queue = this->getOrAddRenderQueue(queueID);
        queue.addItem(renderer, renderable, isStatic, isActive, layer);
    }

    void RenderQueueManager::addMeshToQueue(UInt32 queueID, WeakPointer<MeshRenderer> meshRenderer, WeakPointer<Mesh> mesh,
                                            Bool isStatic, Bool isActive, Int32 layer) {
        RenderQueue& queue = this->getOrAddRenderQueue(queueID);
        queue.addMesh(meshRenderer, mesh, isStatic, isActive, layer);
    }

    void RenderQueueManager::addParticleSystemToQueue(UInt32 queueID, WeakPointer<ParticleSystemRenderer> particleSystemRenderer,
                                                      WeakPointer<ParticleSystem> particleSystem, Bool isStatic, Bool isActive, Int32 layer) {
        RenderQueue& queue = this->getOrAddRenderQueue(queueID);
        queue.addParticleSystem(particleSystemRenderer, particleSystem, isStatic, isActive, layer);
    }

    RenderQueue& RenderQueueManager::getOrAddRenderQueue(UInt32 queueID) {
        if (this->queues.find(queueID) == this->queues.end()) {
            this->queues.emplace(queueID, queueID);
            RenderQueue& queue = queues.at(queueID);
            this->orderedQueues.push_back(&queue);
            Int32 testIndex = this->orderedQueues.size() - 2;
            while (testIndex >= 0) {
                RenderQueue& testQueue = *(this->orderedQueues[testIndex]);
                if (testQueue.getID() > queueID) {
                   std::iter_swap(this->orderedQueues.begin() + testIndex, this->orderedQueues.begin() + testIndex + 1);
                }
                testIndex--;
            }
        }
        RenderQueue& queue = this->queues.at(queueID);
    }

    RenderQueue& RenderQueueManager::getRenderQueue(UInt32 index) {
        if (index >= this->getRenderQueueCount()) {
            throw OutOfRangeException("RenderQueueManager::getRenderQueue -> Index is out of bounds.");
        }
        return *(this->orderedQueues[index]);
    }

}