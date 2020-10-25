#pragma once

#include "../common/types.h"
#include "../base/CoreObject.h"

namespace Core {

    class IndexBuffer: public CoreObject {
    public:
        IndexBuffer(UInt32 size);
        virtual ~IndexBuffer();
        virtual Int32 getBufferID() const = 0;
        virtual void initIndices() = 0;
        virtual void setIndices(UInt32 * indices);
        UInt32 getIndex(UInt32 offset);
        UInt32 getSize();

    protected:
        UInt32 size;
        UInt32 *indices;
    };

}
