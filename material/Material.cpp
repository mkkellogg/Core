#include <memory>

#include "Material.h"
#include "Shader.h"
#include "../Graphics.h"
#include "../render/EngineRenderQueue.h"
#include "../common/debug.h"

namespace Core {

    Material::Material(WeakPointer<Graphics> graphics): graphics(graphics) {
        this->ready = false;
        this->colorWriteEnabled = true;

        this->blendingMode = RenderState::BlendingMode::None;
        this->blendingEquation = RenderState::BlendingEquation::Add;
        this->srcBlendingFactor = RenderState::BlendingFactor::One;
        this->destBlendingFactor = RenderState::BlendingFactor::Zero;
        this->renderStyle = RenderStyle::Fill;
        this->renderQueueID = (UInt16)EngineRenderQueue::Geometry;

        this->lit = false;
        this->physical = false;
        this->skinningEnabled = false;
        
        this->depthWriteEnabled = true;
        this->depthTestEnabled = true;
        this->depthFunction = RenderState::DepthFunction::LessThanOrEqual;

        this->faceCullingEnabled = true;
        this->cullFace = RenderState::CullFace::Back;

        this->stencilTestEnabled = false;
        this->stencilReadMask = 0xFF;
        this->stencilWriteMask = 0xFF;
    }

    Material::Material(WeakPointer<Graphics> graphics, WeakPointer<Shader> shader): Material(graphics) {
        this->setShader(shader);
    }

    WeakPointer<Shader> Material::getShader() {
        return this->shader;
    }

    UInt32 Material::textureCount() {
        return 0;
    }

    void Material::sendCustomUniformsToShader() {

    }

    Bool Material::getColorWriteEnabled() const {
        return this->colorWriteEnabled;
    }

    void Material::setColorWriteEnabled(Bool enabled) {
        this->colorWriteEnabled = enabled;
    }

    RenderState::BlendingEquation Material::getBlendingEquation() const {
        return this->blendingEquation;
    }

    void Material::setBlendingEquation(RenderState::BlendingEquation equation) {
        this->blendingEquation = equation;
    }

    RenderState::BlendingFactor Material::getSourceBlendingFactor() const {
        return this->srcBlendingFactor;
    }

    void Material::setSourceBlendingFactor(RenderState::BlendingFactor factor) {
        this->srcBlendingFactor = factor;
    }

    RenderState::BlendingFactor Material::getDestBlendingFactor() const {
        return this->destBlendingFactor;
    }

    void Material::setDestBlendingFactor(RenderState::BlendingFactor factor) {
        this->destBlendingFactor = factor;
    }

    RenderStyle Material::getRenderStyle() const {
        return this->renderStyle;
    }

    void Material::setRenderStyle(RenderStyle style) {
        this->renderStyle = style;
    }

    RenderState::BlendingMode Material::getBlendingMode() const {
        return this->blendingMode;
    }

    void Material::setBlendingMode(RenderState::BlendingMode mode) {
        this->blendingMode = mode;
    }

    UInt16 Material::getRenderQueueID() const {
        return this->renderQueueID;
    }

    void Material::setRenderQueueID(UInt16 renderQueueID) {
        this->renderQueueID = renderQueueID;
    }

    Bool Material::isLit() const {
        return this->lit;
    }

    void Material::setLit(Bool lit) {
        this->lit = lit;
    }

    Bool Material::isPhysical() const {
        return this->physical;
    }

    void Material::setPhysical(Bool physical) {
        this->physical = physical;
    }

    void Material::setSkinningEnabled(Bool enabled) {
        this->skinningEnabled = enabled;
    }

    Bool Material::isSkinningEnabled() const {
        return this->skinningEnabled;
    }

    Bool Material::getDepthWriteEnabled() const {
        return this->depthWriteEnabled;
    }

    void Material::setDepthWriteEnabled(Bool enabled) {
        this->depthWriteEnabled = enabled;
    }

    Bool Material::getDepthTestEnabled() const {
        return this->depthTestEnabled;
    }

    void Material::setDepthTestEnabled(Bool enabled) {
        this->depthTestEnabled = enabled;
    }

    RenderState::DepthFunction Material::getDepthFunction() {
        return this->depthFunction;
    }

    void Material::setDepthFunction(RenderState::DepthFunction depthFunction) {
        this->depthFunction = depthFunction;
    }

    Bool Material::getFaceCullingEnabled() const {
        return this->faceCullingEnabled;
    }

    void Material::setFaceCullingEnabled(Bool enabled) {
        this->faceCullingEnabled = enabled;
    }

    RenderState::CullFace Material::getCullFace() {
        return this->cullFace;
    }

    void Material::setCullFace(RenderState::CullFace cullFace) {
        this->cullFace = cullFace;
    }

    Bool Material::getStencilTestEnabled() const {
        return this->stencilTestEnabled;
    }

    void Material::setStencilTestEnabled(Bool enabled) {
        this->stencilTestEnabled = enabled;
    }
    
    Byte Material::getStencilRef() const {
        return this->stencilRef;
    }
    
    void Material::setStencilRef(Byte ref) {
        this->stencilRef = ref;
    }
    
    Byte Material::getStencilReadMask() const {
        return this->stencilReadMask;
    }
    
    void Material::setStencilReadMask(Byte mask) {
        this->stencilReadMask = mask;
    }
    
    Byte Material::getStencilWriteMask() const {
        return this->stencilWriteMask;
    }
    
    void Material::setStencilWriteMask(Byte mask) {
        this->stencilWriteMask = mask;
    }
    
    RenderState::StencilFunction Material::getStencilComparisonFunction() const {
        return this->stencilComparisonFunction;
    }
    
    void Material::setStencilComparisonFunction(RenderState::StencilFunction function) {
        this->stencilComparisonFunction = function;
    }
    
    RenderState::StencilAction Material::getStencilAllPassAction() const {
        return this->stencilAllPassAction;
    }
    
    void Material::setStencilAllPassAction(RenderState::StencilAction action) {
        this->stencilAllPassAction = action;
    }
    
    RenderState::StencilAction Material::getStencilFailActionStencil() const {
        return this->stencilFailActionStencil;
    }
    
    void Material::setStencilFailActionStencil(RenderState::StencilAction action) {
        this->stencilFailActionStencil = action;
    }
    
    RenderState::StencilAction Material::getStencilFailActionDepth() const {
        return this->stencilFailActionDepth;
    }
    
    void Material::setStencilFailActionDepth(RenderState::StencilAction action) {
        this->stencilFailActionDepth = action;
    }
    

    void Material::setShader(WeakPointer<Shader> shader) {
        this->shader = shader;
        this->ready = this->shader && this->shader->isReady();
    }

    void Material::copyTo(WeakPointer<Material> target) {
        target->ready = this->ready;
        target->shader = this->shader;

        target->colorWriteEnabled = this->colorWriteEnabled;
        target->blendingMode = this->blendingMode;
        target->srcBlendingFactor = this->srcBlendingFactor;
        target->destBlendingFactor = this->destBlendingFactor;
        target->renderStyle = this->renderStyle;
        target->renderQueueID = this->renderQueueID;
        target->lit = this->lit;
        target->physical = this->physical;
        target->skinningEnabled = this->skinningEnabled;

        target->stencilTestEnabled = this->stencilTestEnabled;
        target->stencilRef = this->stencilRef;
        target->stencilReadMask = this->stencilReadMask;
        target->stencilWriteMask = this->stencilWriteMask;
        target->stencilComparisonFunction = this->stencilComparisonFunction;
        target->stencilAllPassAction = this->stencilAllPassAction;
        target->stencilFailActionStencil = this->stencilFailActionStencil;
        target->stencilFailActionDepth = this->stencilFailActionDepth;

        target->depthWriteEnabled = this->depthWriteEnabled;
        target->depthTestEnabled = this->depthTestEnabled;
        target->depthFunction = this->depthFunction;

        target->faceCullingEnabled = this->faceCullingEnabled;
        target->cullFace = this->cullFace;
    }

    Bool Material::buildFromSource(const std::string& vertexSource, const std::string& fragmentSource) {
        WeakPointer<Shader> shader = this->graphics->createShader(vertexSource, fragmentSource);
        Bool success = shader->build();
        if (!success) {
            this->ready = false;
            return false;
        }
        this->setShader(shader);
        return true;
    }

    Bool Material::buildFromSource(const std::string& vertexSource, const std::string& geometrySource, const std::string& fragmentSource) {
        WeakPointer<Shader> shader = this->graphics->createShader(vertexSource, geometrySource, fragmentSource);
        Bool success = shader->build();
        if (!success) {
            this->ready = false;
            return false;
        }
        this->setShader(shader);
        return true;
    }
}