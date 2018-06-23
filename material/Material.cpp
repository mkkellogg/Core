#include <memory>

#include "Material.h"
#include "Shader.h"
#include "../common/debug.h"

namespace Core {

    Material::Material(Engine& engine, WeakPointer<Graphics> graphics) : graphics(graphics), ready(false), engine(engine) {
    }

    Material::Material(Engine& engine, WeakPointer<Graphics> graphics, WeakPointer<Shader> shader): Material(engine, graphics) {
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
}