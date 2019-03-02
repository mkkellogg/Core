#pragma once

#include "../util/PersistentWeakPointer.h"
#include "../base/BitMask.h"
#include "../math/Matrix4x4.h"

namespace Core {

    // forward declarations
    class Material;
    class RenderTarget;
    class Skybox;

    class ViewDescriptor {
    public:

        Point3r cameraPosition;
        Matrix4x4 viewMatrix;
        Matrix4x4 viewInverseMatrix;
        Matrix4x4 viewInverseTransposeMatrix;
        Matrix4x4 projectionMatrix;
        PersistentWeakPointer<Material> overrideMaterial;
        PersistentWeakPointer<RenderTarget> renderTarget;
        Int32 cubeFace = -1;
        IntMask clearRenderBuffers;
        Skybox* skybox = nullptr;

    };

}