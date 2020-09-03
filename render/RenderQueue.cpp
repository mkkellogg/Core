#include "RenderQueue.h"
#include "../common/Exception.h"

namespace Core {

    RenderQueue::RenderQueue(UInt32 id) {
        this->id = id;
    }

    UInt32 RenderQueue::getID() const {
        return this->id;
    }

    UInt32 RenderQueue::getItemCount() const {
        return this->renderItems.size();
    }

    void RenderQueue::clear() {
        this->renderItems.clear();
        this->renderItemPool.returnAll();
    }

    void RenderQueue::addItem(WeakPointer<BaseObjectRenderer> objectRenderer, WeakPointer<BaseRenderable> renderable) {
        RenderItem& renderItem = this->renderItemPool.acquireObject();
        renderItem.ObjectRenderer = objectRenderer;
        renderItem.Renderable = renderable;
        this->renderItems.push_back(renderItem);
    }

    RenderQueue::RenderItem& RenderQueue::getRenderItem(UInt32 index) {
        if (index >= this->getItemCount()) {
            throw OutOfRangeException("RenderQueue::getRenderItem -> Index is out of bounds.");
        }
        return this->renderItems[index];
    }

}