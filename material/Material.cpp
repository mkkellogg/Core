#include <memory>

#include "Material.h"
#include "Shader.h"
#include "../common/debug.h"

namespace Core {

    Material::Material(WeakPointer<Graphics> graphics): graphics(graphics) {
        this->ready = false;
        this->colorWriteEnabled = true;
        this->blendingMode = RenderState::BlendingMode::Additive;
        this->srcBlendingMethod = RenderState::BlendingMethod::One;
        this->destBlendingMethod = RenderState::BlendingMethod::Zero;
        this->renderStyle = RenderStyle::Fill;
        this->transparent = false;
        this->lit = false;
        
        this->depthWriteEnabled = true;
        this->depthTestEnabled = true;
        this->depthFunction = RenderState::DepthFunction::LessThanOrEqual;
    }

    Material::Material(WeakPointer<Graphics> graphics, WeakPointer<Shader> shader): Material(graphics) {
        this->setShader(shader);
    }

    void Material::setShader(WeakPointer<Shader> shader) {
        this->shader = shader;
        this->ready = this->shader && this->shader->isReady();
    }

    WeakPointer<Shader> Material::getShader() {
        return this->shader;
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

    UInt32 Material::textureCount() {
        return 0;
    }

    Bool Material::getColorWriteEnabled() const {
        return this->colorWriteEnabled;
    }

    void Material::setColorWriteEnabled(Bool enabled) {
        this->colorWriteEnabled = enabled;
    }

    RenderState::BlendingMethod Material::getSourceBlendingMethod() const {
        return this->srcBlendingMethod;
    }

    void Material::setSourceBlendingMethod(RenderState::BlendingMethod method) {
        this->srcBlendingMethod = method;
    }

    RenderState::BlendingMethod Material::getDestBlendingMethod() const {
        return this->destBlendingMethod;
    }

    void Material::setDestBlendingMethod(RenderState::BlendingMethod method) {
        this->destBlendingMethod = method;
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

    Bool Material::isTransparent() const {
        return this->transparent;
    }

    void Material::setTransparent(Bool transparent) {
        this->transparent = transparent;
    }

    Bool Material::isLit() const {
        return this->lit;
    }

    void Material::setLit(Bool lit) {
        this->lit = lit;
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
}