//
// Created by Mark Kellogg on 1/3/18.
//

#include "Color4Components.h"

namespace Core {

  Color4Components::Color4Components(Real* data, const Real& x, const Real& y, const Real& z, const Real w):
      r(data[0]), g(data[1]), b(data[2]), a(data[2]) {
    this->set(r, g, b, a);
  }

  Color4Components::~Color4Components() {}

  void Color4Components::set(const Real& r, const Real& g, const Real& b, const Real& a) {
    this->r = r;
    this->g = g;
    this->b = b;
    this->a = a;
  }

  void Color4Components::copy(const Color4Components& src) {
    this->set(src.r, src.g, src.b, src.a);
  }

}

