#include "IndexBuffer.h"

namespace Core {

    IndexBuffer::IndexBuffer(UInt32 size) : size(size) {
    }

    IndexBuffer::~IndexBuffer() {
    }

    UInt32 IndexBuffer::getSize() {
        return this->size;
    }
}