#pragma once

#include "../common/types.h"
#include "BaseVector.h"

namespace Core {

  template<typename T, UInt32 componentCount, bool customStorage>
  class VectorStorage {};

  template<typename T, UInt32 componentCount>
  class VectorStorage <T, componentCount, true>: public BaseVector<T, componentCount> {
  public:
    VectorStorage(T* storage): BaseVector<T, componentCount>(storage) {}
  };

  template<typename T, UInt32 componentCount>
  class VectorStorage <T, componentCount, false>: public BaseVector<T, componentCount> {
  public:
    VectorStorage(): BaseVector<T, componentCount>(_storage) {}
  private:
    T _storage[componentCount];
  };

}
