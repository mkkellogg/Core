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

    void RenderList::addItem(WeakPointer<BaseObject3DRenderer> renderer, WeakPointer<BaseRenderable> renderable, Bool isStatic, Bool isActive) {
        RenderItem& renderItem = this->renderItemPool.acquireObject();
        renderItem.meshRenderer =  WeakPointer<MeshRenderer>::nullPtr();
        renderItem.renderer = renderer;
        renderItem.mesh = WeakPointer<Mesh>::nullPtr();
        renderItem.renderable = renderable;
        renderItem.isStatic = isStatic;
        renderItem.isActive = isActive;
        this->renderItems.push_back(&renderItem);
    }

    void RenderList::addMesh(WeakPointer<MeshRenderer> meshRenderer, WeakPointer<Mesh> mesh, Bool isStatic, Bool isActive) {
        RenderItem& renderItem = this->renderItemPool.acquireObject();
        renderItem.meshRenderer = meshRenderer;
        renderItem.renderer = WeakPointer<BaseObject3DRenderer>::nullPtr();
        renderItem.mesh = mesh;
        renderItem.renderable = WeakPointer<BaseRenderable>::nullPtr();
        renderItem.isStatic = isStatic;
        renderItem.isActive = isActive;
        this->renderItems.push_back(&renderItem);
    }

    RenderItem& RenderList::getRenderItem(UInt32 index) {
        if (index >= this->getItemCount()) {
            throw OutOfRangeException("RenderList::getRenderItem -> Index is out of bounds.");
        }
        RenderItem *ptr = this->renderItems[index];
        return *ptr;
    }

    void RenderList::setAllActive() {
        for (UInt32 i = 0; i < this->getItemCount(); i++) {
            this->renderItems[i]->isActive = true;
        }
    }

}