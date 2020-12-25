#pragma once

#include "../common/types.h"

namespace Core {

    class Color4Components {
    public:
        Real& r;
        Real& g;
        Real& b;
        Real& a;

        Color4Components(Real* data, const Real& r, const Real& g, const Real& b, const Real& a);
        virtual ~Color4Components() = 0;

        void set(const Real& r, const Real& g, const Real& b, const Real& a);
    };
}
