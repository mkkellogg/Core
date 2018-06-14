#pragma once

#include "../common/types.h"

namespace Core {

    class IndexBuffer {
    public:
        IndexBuffer(UInt32 size);
        virtual ~IndexBuffer();
        virtual Int32 getBufferID() const = 0;
        virtual void initIndices() = 0;
        virtual void setIndices(UInt32 * indices) = 0;
        UInt32 getSize();

    protected:
        UInt32 size;
    };

}
