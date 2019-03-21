#pragma once

#include "../common/types.h"

namespace Core {

    template <typename T, UInt32 componentCount>
    class BaseVector {
    public:
        static const UInt32 ComponentCount = componentCount;
        typedef T ComponentType;
        
        BaseVector(T* data): data(data){}

        T *getData() { 
            return this->data; 
        }

    protected:
        void copy(const BaseVector<T, componentCount>& other);

        T* data;
    };

    template <typename T, UInt32 componentCount>
    void BaseVector<T, componentCount>::copy(const BaseVector<T, componentCount>& other) {
        for (UInt16 i = 0; i < componentCount; i++) {
            this->data[i] = other.data[i];
        }
    }

}
