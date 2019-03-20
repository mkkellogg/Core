#pragma once

#include "../common/types.h"
#include "BaseVector.h"

namespace Core {

    template <typename T, UInt32 componentCount, bool customStorage>
    class VectorStorage {};

    template <typename T, UInt32 componentCount>
    class VectorStorage<T, componentCount, true> : public BaseVector<T, componentCount> {
    public:
        VectorStorage(T* data): BaseVector<T, componentCount>(data) {}
    };

    template <typename T, UInt32 componentCount>
    class VectorStorage<T, componentCount, false> : public BaseVector<T, componentCount> {
    public:
        VectorStorage(): BaseVector<T, componentCount>(this->data) {}

    protected:
        T data[componentCount];
    };
}
