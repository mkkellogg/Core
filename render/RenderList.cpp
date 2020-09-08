#include "RenderList.h"
#include "../common/Exception.h"

namespace Core {

    RenderList::RenderList() {
    }

    UInt32 RenderList::getItemCount() const {
        return this->renderItems.size();
    }

    void RenderList::clear() {
        this->renderItems.clear();
        this->renderItemPool.returnAll();
    }

    void RenderList::addItem(WeakPointer<BaseObjectRenderer> objectRenderer, WeakPointer<BaseRenderable> renderable, Bool isStatic) {
        RenderItem& renderItem = this->renderItemPool.acquireObject();
        renderItem.ObjectRenderer = objectRenderer;
        renderItem.Renderable = renderable;
        renderItem.IsStatic = isStatic;
        this->renderItems.push_back(&renderItem);
    }

    RenderItem& RenderList::getRenderItem(UInt32 index) {
        if (index >= this->getItemCount()) {
            throw OutOfRangeException("RenderList::getRenderItem -> Index is out of bounds.");
        }
        return *this->renderItems[index];
    }

}