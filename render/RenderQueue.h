#pragma once

#include "../common/types.h"
#include "../util/ObjectPool.h"

namespace Core {

    class RenderQueue {
    public:

        class RenderItem {
        public:
            
        };

        RenderQueue(UInt32 initialCapacity);
        void clear();

    protected:
        ObjectPool<RenderItem> renderItems;

    private:
    };

}