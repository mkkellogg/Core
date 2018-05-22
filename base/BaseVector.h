//
// Created by Mark Kellogg on 12/22/17.
//

#ifndef MPM_BASE_VECTOR_H
#define MPM_BASE_VECTOR_H

#include "../common/types.h"

namespace MPM {

  template<typename T, UInt32 componentCount>
  class BaseVector {
  protected:
    T *data;
  public:

    static const UInt32 ComponentCount = componentCount;
    typedef T ComponentType;

    BaseVector(T *data) : data(data) {}

    const T *getConstData() const {
      return this->data;
    }

    T *getData() {
      return this->data;
    }
  };

}


#endif //MPM_BASE_VECTOR_H
