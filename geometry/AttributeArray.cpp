//
// Created by Mark Kellogg on 12/14/17.
//

#include <new>
#include <string.h>
#include "../common/gl.h"
#include "../common/assert.h"

namespace MPM {
  template<typename T>
  AttributeArray<T>::AttributeArray(UInt32 attributeCount): storage(nullptr), attributes(nullptr), attributeCount(attributeCount) {
    allocate();
    buildGPUBuffer();
  }

  template<typename T>
  AttributeArray<T>::~AttributeArray() {
    destroyGPUBuffer();
    deallocate();
  }

  template<typename T>
  void AttributeArray<T>::allocate() {
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

  template<typename T>
  void AttributeArray<T>::deallocate() {
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

  template <typename T>
  void AttributeArray<T>::buildGPUBuffer() {
    glGenBuffers(1, &this->bufferID);
    this->updateBufferData();
  }

  template <typename T>
  void AttributeArray<T>::destroyGPUBuffer() {
    glDeleteBuffers(1, &this->bufferID);
  }

  template<typename T>
  UInt32 AttributeArray<T>::getAttributeCount() const {
    return this->attributeCount;
  }

  template<typename T>
  T& AttributeArray<T>::getAttribute(UInt32 index) {
    return this-attributes[index];
  }

  template<typename T>
  UInt32 AttributeArray<T>::getComponentCount() const {
    return this->componentCount;
  }

  template<typename T>
  GLuint AttributeArray<T>::getGLBufferID() const {
    return this->bufferID;
  }

  template<typename T>
  UInt32 AttributeArray<T>::getSize() const {
    return this->attributeCount * T::ComponentCount * sizeof(typename T::ComponentType);
  }

  template<typename T>
  typename T::ComponentType * AttributeArray<T>::getStorage() {
    return this->storage;
  }

  template<typename T>
  T* AttributeArray<T>::getAttributes() {
    return this->attributes;
  }

  template<typename T>
  void AttributeArray<T>::store(const typename T::ComponentType * data) {
    memcpy(this->storage, data, this->getSize());
    this->updateBufferData();
  }

  template<typename T>
  void AttributeArray<T>::updateBufferData() {
    glBindBuffer(GL_ARRAY_BUFFER, this->bufferID);
    glBufferData(GL_ARRAY_BUFFER, this->getSize(), this->storage, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
  }

  template<typename T>
  typename AttributeArray<T>::iterator AttributeArray<T>::begin() {
    return iterator(this, 0);
  };

  template<typename T>
  typename AttributeArray<T>::iterator AttributeArray<T>::end() {
    return iterator(this, this->getAttributeCount());
  };
}

