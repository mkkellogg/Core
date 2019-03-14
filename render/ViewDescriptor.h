#pragma once

#include "../util/PersistentWeakPointer.h"
#include "../base/BitMask.h"
#include "../math/Matrix4x4.h"
#include "ToneMapType.h"

namespace Core {

    // forward declarations
    class Material;
    class RenderTarget;
    class RenderTarget2D;
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
        PersistentWeakPointer<RenderTarget2D> hdrRenderTarget;
        Int32 cubeFace = -1;
        IntMask clearRenderBuffers;
        Skybox* skybox = nullptr;
        Bool indirectHDREnabled = false;
        ToneMapType hdrToneMapType = ToneMapType::Reinhard;
        Real exposure = 1.0f;
    };

}