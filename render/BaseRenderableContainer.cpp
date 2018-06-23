#include "BaseRenderableContainer.h"
#include "BaseObjectRenderer.h"

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

}
