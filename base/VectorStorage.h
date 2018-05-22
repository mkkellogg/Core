//
// Created by Mark Kellogg on 12/13/17.
//

#ifndef MPM_BASE_VECTOR_STORAGE_H
#define MPM_BASE_VECTOR_STORAGE_H

#include "../common/types.h"
#include "BaseVector.h"

namespace MPM {

  template<typename T, UInt32 componentCount, bool customStorage>
  class VectorStorage {};

  template<typename T, UInt32 componentCount>
  class VectorStorage <T, componentCount, true>: public BaseVector<T, componentCount> {
  public:
    VectorStorage(T* storage): BaseVector<T, componentCount>(storage) {}
  };

  template<typename T, UInt32 componentCount>
  class VectorStorage <T, componentCount, false>: public BaseVector<T, componentCount> {
    T _storage[componentCount];
  public:
    VectorStorage(): BaseVector<T, componentCount>(_storage) {}
  };

}

#endif //MPM_BASE_VECTOR_STORAGE_H
