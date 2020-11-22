#include <memory>

#include "Material.h"
#include "Shader.h"
#include "../Graphics.h"
#include "../render/EngineRenderQueue.h"
#include "../common/debug.h"

namespace Core {

    Material::Material(WeakPointer<Graphics> graphics): graphics(graphics) {
        this->lit = false;
        this->physical = false;
        this->skinningEnabled = false;
        this->renderPath = RenderPath::Forward;
        this->renderQueueID = (UInt16)EngineRenderQueue::Geometry;
        this->ready = false;
        this->customDepthOutput = false;
        this->customDepthOutputCopyOverrideMaterialState = false;
        this->customDepthOutputStateCopyExcludeFaceCulling = false;
    }

    Material::Material(WeakPointer<Graphics> graphics, WeakPointer<Shader> shader): Material(graphics) {
        this->setShader(shader);
    }

    Material::~Material() {}

    WeakPointer<Shader> Material::getShader() {
        return this->shader;
    }

    UInt32 Material::textureCount() {
        return 0;
    }

    void Material::sendCustomUniformsToShader() {

    }

    Bool Material::getColorWriteEnabled() const {
        return this->materialState.colorWriteEnabled;
    }

    void Material::setColorWriteEnabled(Bool enabled) {
        this->materialState.colorWriteEnabled = enabled;
    }

    RenderState::BlendingEquation Material::getBlendingEquation() const {
        return this->materialState.blendingEquation;
    }

    void Material::setBlendingEquation(RenderState::BlendingEquation equation) {
        this->materialState.blendingEquation = equation;
    }

    RenderState::BlendingFactor Material::getSourceBlendingFactor() const {
        return this->materialState.srcBlendingFactor;
    }

    void Material::setSourceBlendingFactor(RenderState::BlendingFactor factor) {
        this->materialState.srcBlendingFactor = factor;
    }

    RenderState::BlendingFactor Material::getDestBlendingFactor() const {
        return this->materialState.destBlendingFactor;
    }

    void Material::setDestBlendingFactor(RenderState::BlendingFactor factor) {
        this->materialState.destBlendingFactor = factor;
    }

    RenderStyle Material::getRenderStyle() const {
        return this->materialState.renderStyle;
    }

    void Material::setRenderStyle(RenderStyle style) {
        this->materialState.renderStyle = style;
    }

    RenderPath Material::getRenderPath() const {
        return this->renderPath;
    }

    void Material::setRenderPath(RenderPath path) {
        this->renderPath = path;
    }

    RenderState::BlendingMode Material::getBlendingMode() const {
        return this->materialState.blendingMode;
    }

    void Material::setBlendingMode(RenderState::BlendingMode mode) {
        this->materialState.blendingMode = mode;
    }

    UInt16 Material::getRenderQueueID() const {
        return this->renderQueueID;
    }

    void Material::setRenderQueue(EngineRenderQueue queue) {
        this->renderQueueID = (UInt16) queue;
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

    UInt32 Material::maxLightCount() const {
        return 1;
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
        return this->materialState.depthWriteEnabled;
    }

    void Material::setDepthWriteEnabled(Bool enabled) {
        this->materialState.depthWriteEnabled = enabled;
    }

    Bool Material::getDepthTestEnabled() const {
        return this->materialState.depthTestEnabled;
    }

    void Material::setDepthTestEnabled(Bool enabled) {
        this->materialState.depthTestEnabled = enabled;
    }

    RenderState::DepthFunction Material::getDepthFunction() {
        return this->materialState.depthFunction;
    }

    void Material::setDepthFunction(RenderState::DepthFunction depthFunction) {
        this->materialState.depthFunction = depthFunction;
    }

    Bool Material::getFaceCullingEnabled() const {
        return this->materialState.faceCullingEnabled;
    }

    void Material::setFaceCullingEnabled(Bool enabled) {
        this->materialState.faceCullingEnabled = enabled;
    }

    RenderState::CullFace Material::getCullFace() const {
        return this->materialState.cullFace;
    }

    void Material::setCullFace(RenderState::CullFace cullFace) {
        this->materialState.cullFace = cullFace;
    }

    Bool Material::getStencilTestEnabled() const {
        return this->materialState.stencilTestEnabled;
    }

    void Material::setStencilTestEnabled(Bool enabled) {
        this->materialState.stencilTestEnabled = enabled;
    }
    
    Byte Material::getStencilRef() const {
        return this->materialState.stencilRef;
    }
    
    void Material::setStencilRef(Byte ref) {
        this->materialState.stencilRef = ref;
    }
    
    Byte Material::getStencilReadMask() const {
        return this->materialState.stencilReadMask;
    }
    
    void Material::setStencilReadMask(Byte mask) {
        this->materialState.stencilReadMask = mask;
    }
    
    Byte Material::getStencilWriteMask() const {
        return this->materialState.stencilWriteMask;
    }
    
    void Material::setStencilWriteMask(Byte mask) {
        this->materialState.stencilWriteMask = mask;
    }
    
    RenderState::StencilFunction Material::getStencilComparisonFunction() const {
        return this->materialState.stencilComparisonFunction;
    }
    
    void Material::setStencilComparisonFunction(RenderState::StencilFunction function) {
        this->materialState.stencilComparisonFunction = function;
    }
    
    RenderState::StencilAction Material::getStencilAllPassAction() const {
        return this->materialState.stencilAllPassAction;
    }
    
    void Material::setStencilAllPassAction(RenderState::StencilAction action) {
        this->materialState.stencilAllPassAction = action;
    }
    
    RenderState::StencilAction Material::getStencilFailActionStencil() const {
        return this->materialState.stencilFailActionStencil;
    }
    
    void Material::setStencilFailActionStencil(RenderState::StencilAction action) {
        this->materialState.stencilFailActionStencil = action;
    }
    
    RenderState::StencilAction Material::getStencilFailActionDepth() const {
        return this->materialState.stencilFailActionDepth;
    }
    
    void Material::setStencilFailActionDepth(RenderState::StencilAction action) {
        this->materialState.stencilFailActionDepth = action;
    }
    

    void Material::setShader(WeakPointer<Shader> shader) {
        this->shader = shader;
        this->ready = this->shader && this->shader->isReady();
    }

    void Material::copyTo(WeakPointer<Material> target) {
        target->ready = this->ready;
        target->shader = this->shader;
        target->lit = this->lit;
        target->physical = this->physical;
        target->skinningEnabled = this->skinningEnabled;

        target->materialState = this->materialState;

        target->customDepthOutput = this->customDepthOutput;
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

    void Material::setCustomDepthOutput(Bool customDepthOutput) {
        this->customDepthOutput = customDepthOutput;
    }

    Bool Material::hasCustomDepthOutput() const {
        return this->customDepthOutput;
    }

    Bool Material::getCustomDepthOutputCopyOverrideMatrialState() const {
        return this->customDepthOutputCopyOverrideMaterialState;
    }

    void Material::setCustomDepthOutputCopyOverrideMatrialState(Bool state) {
        this->customDepthOutputCopyOverrideMaterialState = state;
    }

    Bool Material::getCustomDepthOutputStateCopyExcludeFaceCulling() const {
        return this->customDepthOutputStateCopyExcludeFaceCulling;
    }

    void Material::setCustomDepthOutputStateCopyExcludeFaceCulling(Bool exclude) {
        this->customDepthOutputStateCopyExcludeFaceCulling = exclude;
    }

    Bool Material::hasOpacityMap() const {
        return false;
    }

    WeakPointer<Texture> Material::getOpacityMap() {
        return WeakPointer<Texture>::nullPtr();
    }

    MaterialState Material::getState() const {
        return this->materialState;
    }

    void Material::setState(MaterialState state) {
        this->materialState = state;
    }

}