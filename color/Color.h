//
// Created by Mark Kellogg on 12/13/17.
//

#ifndef MPM_COLOR_H
#define MPM_COLOR_H

#include "../common/types.h"
#include "../base/VectorStorage.h"
#include "Color4Components.h"

namespace Core {

  #define COLOR_COMPONENT_COUNT 4

  template <bool customStorage> class Color4 :
      public VectorStorage<Real, COLOR_COMPONENT_COUNT, customStorage>, public Color4Components {

  public:

    Color4();
    Color4(Real r, Real g, Real b, Real a);
    Color4(Real* storage);
    Color4(Real* storage, Real r, Real g, Real b, Real a);

  };

  typedef Color4<false> Color;
  typedef Color4<true> ColorS;
}

#include "Color.cpp"

#endif //MPM_COLOR_H
