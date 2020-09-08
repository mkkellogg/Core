#include "RenderQueue.h"

namespace Core {

    RenderQueue::RenderQueue(UInt32 id) {
        this->id = id;
    }

    UInt32 RenderQueue::getID() const {
        return this->id;
    }

}