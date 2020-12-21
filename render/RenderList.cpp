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

    void RenderList::addItem(WeakPointer<BaseObject3DRenderer> renderer, WeakPointer<BaseRenderable> renderable, Bool isStatic, Bool isActive, Int32 layer) {
        RenderItem& renderItem = this->renderItemPool.acquireObject();
        this->initRenderItem(renderItem, isStatic, isActive, layer);
        renderItem.renderer = renderer;
        renderItem.renderable = renderable;
        this->renderItems.push_back(&renderItem);
    }

    void RenderList::addMesh(WeakPointer<MeshRenderer> meshRenderer, WeakPointer<Mesh> mesh, Bool isStatic, Bool isActive, Int32 layer) {
        RenderItem& renderItem = this->renderItemPool.acquireObject();
        this->initRenderItem(renderItem, isStatic, isActive, layer);
        renderItem.meshRenderer = meshRenderer;
        renderItem.mesh = mesh;
        this->renderItems.push_back(&renderItem);
    }

    void RenderList::addParticleSystem(WeakPointer<ParticleSystemRenderer> particleSystemRenderer, WeakPointer<ParticleSystem> particleSystem, Bool isStatic, Bool isActive, Int32 layer) {
        RenderItem& renderItem = this->renderItemPool.acquireObject();
        this->initRenderItem(renderItem, isStatic, isActive, layer);
        renderItem.particleSystemRenderer = particleSystemRenderer;
        renderItem.particleSystem = particleSystem;
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

    void RenderList::initRenderItem(RenderItem& renderItem, Bool isStatic, Bool isActive, Int32 layer) {
        renderItem.meshRenderer = WeakPointer<MeshRenderer>::nullPtr();
        renderItem.renderer = WeakPointer<BaseObject3DRenderer>::nullPtr();
        renderItem.particleSystem = WeakPointer<ParticleSystem>::nullPtr();
        renderItem.mesh = WeakPointer<Mesh>::nullPtr();
        renderItem.renderable = WeakPointer<BaseRenderable>::nullPtr();
        renderItem.particleSystemRenderer = WeakPointer<ParticleSystemRenderer>::nullPtr();
        renderItem.isStatic = isStatic;
        renderItem.isActive = isActive;
        renderItem.layer = layer;
    }

}