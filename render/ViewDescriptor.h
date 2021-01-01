#pragma once

#include "../util/PersistentWeakPointer.h"
#include "../base/BitMask.h"
#include "../math/Matrix4x4.h"
#include "ToneMapType.h"
#include "DepthOutputOverride.h"

namespace Core {

    // forward declarations
    class Material;
    class RenderTarget;
    class RenderTarget2D;
    class Skybox;
    class Texture2D;

    class ViewDescriptor {
    public:

        Point3r cameraPosition;
        Matrix4x4 inverseCameraTransformation;
        Matrix4x4 cameraTransformation;
        Matrix4x4 transposedCameraTransformation;
        Matrix4x4 projectionMatrix;
        mutable PersistentWeakPointer<Material> overrideMaterial;
        PersistentWeakPointer<RenderTarget> renderTarget;
        PersistentWeakPointer<RenderTarget> hdrRenderTarget;
        Int32 cubeFace = -1;
        Int32 mipLevel = 0;
        IntMask clearRenderBuffers;
        Skybox* skybox = nullptr;
        Bool indirectHDREnabled = false;
        ToneMapType hdrToneMapType = ToneMapType::Reinhard;
        Real hdrExposure = 1.0f;
        Real hdrGamma = 2.2f;
        Bool ssaoEnabled = false;
        WeakPointer<Texture2D> ssaoMap;
        Real ssaoRadius = 1.5f;
        Real ssaoBias = 0.05f;
        DepthOutputOverride depthOutputOverride = DepthOutputOverride::None;
    };

}