//
// Created by Mark Kellogg on 12/13/17.
//

#ifndef MPM_COLOR4_COMPONENTS_H
#define MPM_COLOR4_COMPONENTS_H

#include "../common/types.h"

namespace MPM {

  class Color4Components {
  public:
    Real& r;
    Real& g;
    Real& b;
    Real& a;

    Color4Components(Real* data, const Real& x, const Real& y, const Real& z, const Real w);
    virtual ~Color4Components() = 0;

    void set(const Real& r, const Real& g, const Real& b, const Real& a);
    void copy(const Color4Components& src);

  };

}

#endif //MPM_COLOR4_COMPONENTS_H
