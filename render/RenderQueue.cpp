#include "RenderQueue.h"

namespace Core {

    RenderQueue::RenderQueue(UInt32 initialCapacity): renderItems(initialCapacity) {

    }

    void RenderQueue::clear() {
        this->renderItems.returnAll();
    }

}