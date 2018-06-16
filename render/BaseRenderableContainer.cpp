#include "BaseRenderableContainer.h"

namespace Core {

    BaseRenderableContainer::~BaseRenderableContainer() {
        
    }

    std::weak_ptr<BaseObjectRenderer> BaseRenderableContainer::getBaseRenderer() {
       return  this->renderer;
    }

    void BaseRenderableContainer::setBaseRenderer(std::shared_ptr<BaseObjectRenderer> renderer) {
        this->addComponent(renderer);
        this->renderer = renderer;
    }

}
