#pragma once

#include <string.h>
#include <new>

#include "../common/Exception.h"
#include "../common/assert.h"
#include "../common/gl.h"
#include "../common/types.h"

namespace Core {

    template <typename T>
    class AttributeArray {
    public:
        AttributeArray(UInt32 attributeCount) : storage(nullptr), attributes(nullptr), attributeCount(attributeCount) {
            allocate();
        }

        virtual ~AttributeArray() {
            deallocate();
        }

        virtual Int32 getBufferID() const = 0;
        virtual void sendToShader(UInt32 location) = 0;

        UInt32 getComponentCount() const {
            return this->componentCount;
        }

        UInt32 getAttributeCount() const {
            return this->attributeCount;
        }

        typename T::ComponentType* getStorage() {
            return this->storage;
        }

        T* getAttributes() {
            return this->attributes;
        }

        T& getAttribute(UInt32 index) {
            return this - attributes[index];
        }

        void store(const typename T::ComponentType* data) {
            memcpy(this->storage, data, this->getSize());
            this->updateBufferData();
        }

        class iterator {
            AttributeArray<T>* array;
            UInt32 index;

        public:
            iterator(AttributeArray<T>* array, UInt32 index) : array(array), index(index) {
            }

            T& operator*() {
                return array->attributes[index];
            }

            iterator operator++() {
                if (this->index < this->array->getAttributeCount()) {
                    ++this->index;
                }
                return *this;
            }

            iterator operator++(int i) {
                if (this->index < this->array->getAttributeCount()) {
                    this->index++;
                }
                return *this;
            }

            Bool operator==(const iterator& other) {
                if (this == &other) return true;
                return this->index == other.index && this->array == other.array;
            }

            Bool operator!=(const iterator& other) {
                return this->operator==(other);
            }
        };

        iterator begin() {
            return iterator(this, 0);
        }

        iterator end() {
            return iterator(this, this->getAttributeCount());
        }

    protected:
        typename T::ComponentType* storage;
        T* attributes;
        UInt32 attributeCount;
        const UInt32 componentCount = T::ComponentCount;

        UInt32 getSize() const {
            return this->attributeCount * T::ComponentCount * sizeof(typename T::ComponentType);
        }

        virtual void updateBufferData() = 0;

        void allocate() {
            this->deallocate();

            this->storage = new (std::nothrow) typename T::ComponentType[this->attributeCount * T::ComponentCount];
            if (this->storage == nullptr) {
                throw AllocationException("AttributeArray::allocate() -> Unable to allocate storage!");
            }

            Byte* tempAttributes = (Byte*)(::operator new(this->attributeCount * sizeof(T)));
            if (tempAttributes == nullptr) {
                throw AllocationException("AttributeArray::allocate() -> Unable to allocate attributes!");
            }

            this->attributes = reinterpret_cast<T*>(tempAttributes);
            for (UInt32 i = 0; i < this->attributeCount; i++) {
                new (this->attributes + i) T(this->storage + i);
            }
        }

        void deallocate() {
            if (this->attributes != nullptr) {
                for (UInt32 i = 0; i < this->attributeCount; i++) {
                    this->attributes[i].~T();
                }
                ::operator delete(this->attributes);
                this->attributes = nullptr;
            }
            if (this->storage != nullptr) {
                delete this->storage;
                this->storage = nullptr;
            }
        }
    };
}
