#include "BaseRenderableContainer.h"
#include "BaseObjectRenderer.h"
#include "BaseRenderable.h"

namespace Core {

    BaseRenderableContainer::~BaseRenderableContainer() {
        
    }

    WeakPointer<BaseObjectRenderer> BaseRenderableContainer::getBaseRenderer() {
       return  this->renderer;
    }

    void BaseRenderableContainer::setBaseRenderer(WeakPointer<BaseObjectRenderer> renderer) {
        if (this->addComponent(renderer)) {
            this->renderer = renderer;
        }
    }

    void BaseRenderableContainer::addBaseRenderable(WeakPointer<BaseRenderable> renderable) {
        this->renderables.push_back(renderable);
    }

    UInt32 BaseRenderableContainer::getBaseRenderableCount() const {
        return this->renderables.size();
    }

    WeakPointer<BaseRenderable> BaseRenderableContainer::getBaseRenderable(UInt32 index) {
        if (index >= this->renderables.size()) {
            throw OutOfRangeException("BaseRenderableContainer::getBaseRenderable -> 'index' is out of range.");
        }
        return this->renderables[index];
    }
}
