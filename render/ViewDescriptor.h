#pragma once

#include "../util/PersistentWeakPointer.h"
#include "../math/Matrix4x4.h"

namespace Core {

    // forward declarations
    class Material;

    class ViewDescriptor {
    public:

        Matrix4x4 viewMatrix;
        Matrix4x4 viewInverseMatrix;
        Matrix4x4 viewInverseTransposeMatrix;
        Matrix4x4 projectionMatrix;
        PersistentWeakPointer<Material> overrideMaterial;

    };

}