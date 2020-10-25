#pragma once

#include <string.h>
#include <new>

#include "../Engine.h"
#include "../util/WeakPointer.h"
#include "../common/Exception.h"
#include "../common/assert.h"
#include "../common/types.h"
#include "../Graphics.h"
#include "../geometry/AttributeType.h"
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

        WeakPointer<AttributeArrayGPUStorage> getGPUStorage() {
            return this->gpuStorage;
        }

    protected:
        UInt32 attributeCount;
        UInt32 componentCount;
        PersistentWeakPointer<AttributeArrayGPUStorage> gpuStorage;
    };

    template <typename T>
    class AttributeArray final: public AttributeArrayBase {
    public:

        AttributeArray(UInt32 attributeCount) : AttributeArrayBase(attributeCount, T::ComponentCount), storage(nullptr), attributes(nullptr) {
            this->gpuAttributeType = AttributeType::Float;
            this->normalizeGPUData = false;
            this->autoAllocateGPUSorage = false;
            allocate();
        }

        AttributeArray(UInt32 attributeCount, AttributeType gpuAttributeType, Bool normalizeGPUData) : AttributeArrayBase(attributeCount, T::ComponentCount), storage(nullptr), attributes(nullptr) {
            this->gpuAttributeType = gpuAttributeType;
            this->normalizeGPUData = normalizeGPUData;
            this->autoAllocateGPUSorage = true;
            allocate();
        }

        virtual  ~AttributeArray() {
            deallocate();
        }

        typename T::ComponentType* getStorage() {
            return this->storage;
        }

        T* getAttributes() {
            return this->attributes;
        }

        T& getAttribute(UInt32 index) {
            if (index >= this->attributeCount) {
                throw OutOfRangeException("AttributeArray::getAttribute() -> 'index' is out of range.");
            }
            return this->attributes[index];
        }

        void store(const typename T::ComponentType* data) {
            memcpy(this->storage, data, this->getSize());
            this->updateGPUStorageData();
        }

        void setGPUStorage(WeakPointer<AttributeArrayGPUStorage> storage) { 
            this->deallocateGPUStorage();
            this->gpuStorage = storage;
            this->updateGPUStorageData();
        }

        void updateGPUStorageData() {
            if (this->gpuStorage) {
                this->gpuStorage->updateBufferData((void *)this->storage);
            }
        }

        class iterator {
            AttributeArray<T>* array;
            UInt32 index;

        public:
            iterator(AttributeArray<T>* array, UInt32 index) : array(array), index(index) {
            }

            T& operator *() {
                return array->attributes[index];
            }

            iterator operator ++() {
                if (this->index < this->array->getAttributeCount()) {
                    ++this->index;
                }
                return *this;
            }

            iterator operator ++(int i) {
                if (this->index < this->array->getAttributeCount()) {
                    this->index++;
                }
                return *this;
            }

            Bool operator ==(const iterator& other) {
                if (this == &other) return true;
                return this->index == other.index && this->array == other.array;
            }

            Bool operator !=(const iterator& other) {
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
        AttributeType gpuAttributeType;
        Bool normalizeGPUData;
        Bool autoAllocateGPUSorage;
        typename T::ComponentType* storage;
        T* attributes;

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
                new (this->attributes + i) T(this->storage + (i * T::ComponentCount));
            }

            if (this->autoAllocateGPUSorage) this->allocateGPUStorage();
        }

        void allocateGPUStorage() {
            WeakPointer<AttributeArrayGPUStorage> gpuStorage =
            Engine::instance()->createGPUStorage(this->getSize(), T::ComponentCount, this->gpuAttributeType, this->normalizeGPUData);
            this->setGPUStorage(gpuStorage);
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
            this->deallocateGPUStorage();
        }

        void deallocateGPUStorage() {
            if(this->gpuStorage.isValid()) {
               Engine::safeReleaseObject(this->gpuStorage);
            }
        }
    };

    template <typename T>
    class ScalarAttributeArray final: public AttributeArrayBase {
    public:

        ScalarAttributeArray(UInt32 attributeCount) : AttributeArrayBase(attributeCount, 1), attributes(nullptr) {
            allocate();
        }

        virtual  ~ScalarAttributeArray() {
            deallocate();
        }

        T* getAttributes() {
            return this->attributes;
        }

        T& getAttribute(UInt32 index) {
            if (index >= this->attributeCount) {
                throw OutOfRangeException("AttributeArray::getAttribute() -> 'index' is out of range.");
            }
            return this->attributes[index];
        }

        void store(const T* data) {
            memcpy(this->attributes, data, this->getSize());
            this->updateGPUStorageData();
        }

        void setGPUStorage(WeakPointer<AttributeArrayGPUStorage> storage) { 
            this->deallocateGPUStorage();
            this->gpuStorage = storage;
            this->updateGPUStorageData();
        }

        void updateGPUStorageData() {
            if (this->gpuStorage) {
                this->gpuStorage->updateBufferData((void *)this->attributes);
            }
        }

        class iterator {
            ScalarAttributeArray<T>* array;
            UInt32 index;

        public:
            iterator(ScalarAttributeArray<T>* array, UInt32 index) : array(array), index(index) {
            }

            T& operator *() {
                return array->attributes[index];
            }

            iterator operator ++() {
                if (this->index < this->array->getAttributeCount()) {
                    ++this->index;
                }
                return *this;
            }

            iterator operator ++(int i) {
                if (this->index < this->array->getAttributeCount()) {
                    this->index++;
                }
                return *this;
            }

            Bool operator ==(const iterator& other) {
                if (this == &other) return true;
                return this->index == other.index && this->array == other.array;
            }

            Bool operator !=(const iterator& other) {
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
            return this->attributeCount * sizeof(T);
        }

    protected:
        T* attributes;

        void allocate() {
            this->deallocate();

            this->attributes = new (std::nothrow) T[this->attributeCount];
            if (this->attributes == nullptr) {
                throw AllocationException("ScalarAttributeArray::allocate() -> Unable to allocate storage!");
            }
        }

        void deallocate() {
            if (this->attributes != nullptr) {
                delete this->attributes;
                this->attributes = nullptr;
            }
            this->deallocateGPUStorage();
        }

        void deallocateGPUStorage() {
            if(this->gpuStorage.isValid()) {
               Engine::safeReleaseObject(this->gpuStorage);
            }
        }
    };



}
