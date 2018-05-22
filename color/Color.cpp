//
// Created by Mark Kellogg on 12/13/17.
//

#include "Color4Components.h"

namespace Core {

  template <bool customStorage>
  Color4<customStorage>::Color4(): Color4(0.0, 0.0, 0.0, 1.0) {}

  template <bool customStorage>
  Color4<customStorage>::Color4(Real r, Real g, Real b, Real a): Color4Components(this->data, r, g, b, a) {}

  template <bool customStorage>
  Color4<customStorage>::Color4(Real* storage): Color4(storage, 0.0, 0.0, 0.0, 1.0) {}

  template <bool customStorage>
  Color4<customStorage>::Color4(Real* storage, Real r, Real g, Real b, Real a):
  VectorStorage<Real, COLOR_COMPONENT_COUNT, true>(storage), Color4Components(this->data, r, g, b, a) {}

}


