#pragma once

#include "../common/types.h"

namespace Core {

    class Color4Components {
    public:
        Real& r;
        Real& g;
        Real& b;
        Real& a;

        Color4Components(Real* data, Real r, Real g, Real b, Real a);
        virtual ~Color4Components() = 0;

        void set(Real r, Real g, Real b, Real a);
    };
}
