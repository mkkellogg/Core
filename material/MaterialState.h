
#pragma once

#include <memory>

#include "../util/PersistentWeakPointer.h"
#include "../render/RenderStyle.h"
#include "../render/RenderState.h"
#include "../render/RenderPath.h"

namespace Core {

    class MaterialState {
    public:
        MaterialState();

        Bool colorWriteEnabled;
        RenderState::BlendingMode blendingMode;
        RenderState::BlendingEquation blendingEquation;
        RenderState::BlendingFactor srcBlendingFactor;
        RenderState::BlendingFactor srcAlphaBlendingFactor;
        RenderState::BlendingFactor destBlendingFactor;
        RenderState::BlendingFactor destAlphaBlendingFactor;
        RenderStyle renderStyle;

        Bool stencilTestEnabled;
        Byte stencilRef;
        Byte stencilReadMask;
        Byte stencilWriteMask;
        RenderState::StencilFunction stencilComparisonFunction;
        RenderState::StencilAction stencilAllPassAction;
        RenderState::StencilAction stencilFailActionStencil;
        RenderState::StencilAction stencilFailActionDepth;

        Bool depthWriteEnabled;
        Bool depthTestEnabled;
        RenderState::DepthFunction depthFunction;

        Bool faceCullingEnabled;
        RenderState::CullFace cullFace;
    };
}
