#include "Color4Components.h"

namespace Core {

    Color4Components::Color4Components(Real* data, Real r, Real g, Real b, Real a) : r(data[0]), g(data[1]), b(data[2]), a(data[3]) {
        this->set(r, g, b, a);
    }

    Color4Components::~Color4Components() {
    }

    void Color4Components::set(Real r, Real g, Real b, Real a) {
        this->r = r;
        this->g = g;
        this->b = b;
        this->a = a;
    }
}
