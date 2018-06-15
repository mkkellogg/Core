#include <memory>

#include "Material.h"
#include "../util/WeakPointer.h"
#include "../common/debug.h"

namespace Core {

    Material::Material(std::shared_ptr<Graphics> graphics) : graphics(graphics), ready(false) {
    }

    Material::Material(std::shared_ptr<Graphics> graphics, std::weak_ptr<Shader> shader): Material(graphics) {
        this->setShader(shader);
        WeakPointer<Shader> shaderPtr(shader);
        this->ready = shaderPtr.isInitialized() && shaderPtr->isReady();
    }

    void Material::setShader(std::weak_ptr<Shader> shader) {
        this->shader = shader;
    }

    std::weak_ptr<Shader> Material::getShader() {
        return this->shader;
    }

    Bool Material::buildFromSource(const std::string& vertexSource, const std::string& fragmentSource) {
        std::weak_ptr<Shader> shader = this->graphics->createShader(vertexSource, fragmentSource);
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