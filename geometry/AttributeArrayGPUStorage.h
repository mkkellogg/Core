#pragma once

#include "../common/types.h"

namespace Core {

    class AttributeArrayGPUStorage {
    public:

        virtual ~AttributeArrayGPUStorage() = 0;
        virtual Int32 getBufferID() const = 0;
        virtual void sendToShader(UInt32 location) = 0;
        virtual void disable(UInt32 location) = 0;
        virtual void updateBufferData(void * data) = 0;
    };
}
