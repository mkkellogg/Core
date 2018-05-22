//
// Created by Mark Kellogg on 12/14/17.
//

#ifndef MPM_ATTRIBUTEARRAY_H
#define MPM_ATTRIBUTEARRAY_H

#include "../common/types.h"
#include "../common/gl.h"

namespace Core {

  template <typename T> class AttributeArray {
    typename T::ComponentType * storage;
    T* attributes;
    UInt32 attributeCount;
    const UInt32 componentCount = T::ComponentCount;
    GLuint bufferID;

    void allocate();
    void deallocate();
    void buildGPUBuffer();
    void destroyGPUBuffer();
    void updateBufferData();
    UInt32 getSize() const;

  public:

    AttributeArray(UInt32 attributeCount);
    virtual ~AttributeArray();

    UInt32 getAttributeCount() const;
    UInt32 getComponentCount() const;
    typename T::ComponentType * getStorage();
    T* getAttributes();
    T& getAttribute(UInt32 index);
    void store(const typename T::ComponentType * data);
    GLuint getGLBufferID() const;

    // forward declaration
    class iterator;

    iterator begin();
    iterator end();

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

  };
}

#include "AttributeArray.cpp"

#endif //MPM_ATTRIBUTEARRAY_H
