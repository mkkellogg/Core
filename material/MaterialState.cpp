#include <memory>

#include "MaterialState.h"
#include "../common/debug.h"
#include "../render/EngineRenderQueue.h"

namespace Core {

    MaterialState::MaterialState() {
        this->colorWriteEnabled = true;

        this->blendingMode = RenderState::BlendingMode::None;
        this->blendingEquation = RenderState::BlendingEquation::Add;
        this->srcBlendingFactor = RenderState::BlendingFactor::One;
        this->srcAlphaBlendingFactor = RenderState::BlendingFactor::One;
        this->destBlendingFactor = RenderState::BlendingFactor::Zero;
        this->destAlphaBlendingFactor = RenderState::BlendingFactor::Zero;
        this->renderStyle = RenderStyle::Fill;
        
        this->depthWriteEnabled = true;
        this->depthTestEnabled = true;
        this->depthFunction = RenderState::DepthFunction::LessThanOrEqual;

        this->faceCullingEnabled = true;
        this->cullFace = RenderState::CullFace::Back;

        this->stencilTestEnabled = false;
        this->stencilReadMask = 0xFF;
        this->stencilWriteMask = 0xFF;
    }
}