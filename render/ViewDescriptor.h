#pragma once

#include "../util/PersistentWeakPointer.h"
#include "../base/BitMask.h"
#include "../math/Matrix4x4.h"

namespace Core {

    // forward declarations
    class Material;
    class RenderTarget;

    class ViewDescriptor {
    public:

        Matrix4x4 viewMatrix;
        Matrix4x4 viewInverseMatrix;
        Matrix4x4 viewInverseTransposeMatrix;
        Matrix4x4 projectionMatrix;
        PersistentWeakPointer<Material> overrideMaterial;
        PersistentWeakPointer<RenderTarget> renderTarget;
        Int32 cubeFace = -1;
        IntMask clearRenderBuffers;
    };

}