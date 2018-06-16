#include <memory>

#include "Material.h"
#include "../common/debug.h"

namespace Core {

    Material::Material(WeakPointer<Graphics> graphics) : graphics(graphics), ready(false) {
    }

    Material::Material(WeakPointer<Graphics> graphics, WeakPointer<Shader> shader): Material(graphics) {
        this->setShader(shader);
        WeakPointer<Shader> shaderPtr(shader);
        this->ready = shaderPtr && shaderPtr->isReady();
    }

    void Material::setShader(WeakPointer<Shader> shader) {
        this->shader = shader;
    }

    WeakPointer<Shader> Material::getShader() {
        return this->shader;
    }

    Bool Material::buildFromSource(const std::string& vertexSource, const std::string& fragmentSource) {
         WeakPointer<Graphics> graphicsPtr(this->graphics);
        WeakPointer<Shader> shader = graphicsPtr->createShader(vertexSource, fragmentSource);
        WeakPointer<Shader> shaderPtr(shader);
        Bool success = shaderPtr->build();
        if (!success) {
            this->ready = false;
            return false;
        }
        this->setShader(shader);
        return true;
    }
}