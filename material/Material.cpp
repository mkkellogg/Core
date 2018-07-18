#include <memory>

#include "Material.h"
#include "Shader.h"
#include "../common/debug.h"

namespace Core {

    Material::Material(WeakPointer<Graphics> graphics): graphics(graphics), ready(false) {
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

    UInt32 Material::textureCount() {
        return 0;
    }
}