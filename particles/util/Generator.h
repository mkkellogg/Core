#pragma once

#include "../../common/types.h"
#include "../../math/Math.h"

namespace Core {

    template <typename T>
    class Generator {
    public:
        Generator(){}
        virtual ~Generator(){}
        virtual void generate(T& out) = 0;
        virtual Generator<T>* clone() const = 0;
    };

}