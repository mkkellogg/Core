#pragma once

#include "../common/types.h"
#include "RenderQueue.h"

namespace Core {

    class MaterialGroupedRenderQueue : public RenderQueue{
    public:

        MaterialGroupedRenderQueue(UInt32 initialCapacity);
        void clear();
    };

}