#pragma once

#include "../math/Matrix4x4.h"

namespace Core {

    class ViewDescriptor {
    public:

        Matrix4x4 viewMatrix;
        Matrix4x4 viewInverseTransposeMatrix;
        Matrix4x4 projectionMatrix;

    };

}