#pragma once

#include <new>
#include <string.h>

#include "../common/types.h"
#include "../common/gl.h"
#include "../common/assert.h"

namespace Core {

  template <typename T> class AttributeArray {
    typename T::ComponentType * storage;
    T* attributes;
    UInt32 attributeCount;
    const UInt32 componentCount = T::ComponentCount;
    GLuint bufferID;

    void allocate() {
      this->deallocate();

      this->storage = new (std::nothrow) typename T::ComponentType[this->attributeCount * T::ComponentCount];
      ASSERT(this->storage != nullptr, "AttributeArray::allocate() -> Unable to allocate storage!");

      Byte * tempAttributes =  new(std::nothrow) Byte[this->attributeCount * sizeof(T)];
      ASSERT(tempAttributes != nullptr, "AttributeArray::allocate() -> Unable to allocate attributes!");

      this->attributes = reinterpret_cast<T*>(tempAttributes);
      for (UInt32 i = 0; i < this->attributeCount; i ++) {
        new(this->attributes + i) T(this->storage + i);
      }
    }

    void deallocate() {
      if (this->attributes != nullptr) {
        for (UInt32 i = 0; i < this->attributeCount; i++) {
          this->attributes[i].~T();
        }
        delete this->attributes;
        this->attributes = nullptr;
      }
      if (this->storage != nullptr) {
        delete this->storage;
        this->storage = nullptr;
      }
    }

    void buildGPUBuffer() {
      glGenBuffers(1, &this->bufferID);
      this->updateBufferData();
    }

    void destroyGPUBuffer() {
      glDeleteBuffers(1, &this->bufferID);
    }

    UInt32 getSize() const {
      return this->attributeCount * T::ComponentCount * sizeof(typename T::ComponentType);
    }

    void updateBufferData() {
      glBindBuffer(GL_ARRAY_BUFFER, this->bufferID);
      glBufferData(GL_ARRAY_BUFFER, this->getSize(), this->storage, GL_DYNAMIC_DRAW);
      glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

  public:

    AttributeArray(UInt32 attributeCount): storage(nullptr), attributes(nullptr), attributeCount(attributeCount) {
      allocate();
      buildGPUBuffer();
    }

    ~AttributeArray() {
      destroyGPUBuffer();
      deallocate();
    }

    GLuint getGLBufferID() const {
      return this->bufferID;
    }

    UInt32 getComponentCount() const {
      return this->componentCount;
    }

    UInt32 getAttributeCount() const {
      return this->attributeCount;
    }

    typename T::ComponentType * getStorage() {
      return this->storage;
    }

    T* getAttributes() {
      return this->attributes;
    }

    T& getAttribute(UInt32 index) {
      return this-attributes[index];
    }

    void store(const typename T::ComponentType * data) {
      memcpy(this->storage, data, this->getSize());
      this->updateBufferData();
    }
    
    class iterator {
      AttributeArray<T> * array;
      UInt32 index;
    public:
      iterator(AttributeArray<T> * array, UInt32 index): array(array), index(index) {

      }

      T& operator*() {
        return array->attributes[index];
      }

      const T& operator ++() {
        if (this->index < this->array->getAttributeCount()) {
          ++this->index;
        }
      }

      const T& operator ++(int i) {
        if (this->index < this->array->getAttributeCount()) {
          this->index++;
        }
      }

      Bool operator ==(const iterator& other) {
        if (this == &other) return true;
        return this->index == other.index && this->array == other.array;
      }

      Bool operator !=(const iterator& other) {
        return this->operator ==(other);
      }

    };

    iterator begin() {
      return iterator(this, 0);
    }

    iterator end() {
      return iterator(this, this->getAttributeCount());
    }

  };
}
