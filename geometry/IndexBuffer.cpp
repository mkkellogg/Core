#include "IndexBuffer.h"
#include "../common/Exception.h"
#include "string.h"

namespace Core {

    IndexBuffer::IndexBuffer(UInt32 size) : size(size) {
        this->indices = new (std::nothrow) UInt32[size];
        if (this->indices == nullptr) {
            throw AllocationException("IndexBuffer::IndexBuffer() -> Unable to allocate indices.");
        }
    }   

    IndexBuffer::~IndexBuffer() {
        if (this->indices) {
            delete this->indices;
            this->indices = nullptr;
        }
    }

    void IndexBuffer::setIndices(UInt32 * indices) {
        memcpy(this->indices, indices, sizeof(UInt32) * this->size);
    }

    UInt32 IndexBuffer::getIndex(UInt32 offset) {
        return this->indices[offset];
    }

    UInt32 IndexBuffer::getSize() {
        return this->size;
    }
}