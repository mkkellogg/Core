#pragma once

#include <string.h>
#include <new>

#include "../common/Exception.h"
#include "../common/assert.h"
#include "../common/gl.h"
#include "../common/types.h"
#include "AttributeArrayGPUStorage.h"

namespace Core {

    class AttributeArrayBase {
    public:
        AttributeArrayBase(UInt32 attributeCount,  UInt32 componentCount): attributeCount(attributeCount), componentCount(componentCount) {
        }

        virtual ~AttributeArrayBase() {
         
        }

        UInt32 getComponentCount() const {
            return this->componentCount;
        }

        UInt32 getAttributeCount() const {
            return this->attributeCount;
        }

        std::weak_ptr<AttributeArrayGPUStorage> getGPUStorage() {
            return this->gpuStorage;
        }

    protected:
        UInt32 attributeCount;
        UInt32 componentCount;
        std::shared_ptr<AttributeArrayGPUStorage> gpuStorage;
    };

    template <typename T>
    class AttributeArray: public AttributeArrayBase {
    public:

        AttributeArray(UInt32 attributeCount) : AttributeArrayBase(attributeCount, T::ComponentCount), storage(nullptr), attributes(nullptr) {
            allocate();
        }

        virtual ~AttributeArray() {
            deallocate();
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
            this->updateGPUStorageData();
        }

        void setGPUStorage(std::shared_ptr<AttributeArrayGPUStorage> storage) { 
            this->gpuStorage = storage;
            this->updateGPUStorageData();
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

        UInt32 getSize() const {
            return this->attributeCount * T::ComponentCount * sizeof(typename T::ComponentType);
        }

    protected:
        typename T::ComponentType* storage;
        T* attributes;

        void updateGPUStorageData() {
            if (this->gpuStorage) {
                this->gpuStorage->updateBufferData((void *)this->storage);
            }
        }

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
