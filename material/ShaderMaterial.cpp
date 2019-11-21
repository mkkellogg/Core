#include "ShaderMaterial.h"
#include "../material/Shader.h"
#include "../util/WeakPointer.h"
#include "StandardAttributes.h"
#include "StandardUniforms.h"
#include "../Engine.h"
#include "../Graphics.h"
#include "../material/ShaderManager.h"

namespace Core {

    ShaderMaterial::ShaderMaterial(const std::string& vertexShaderName, const std::string& fragmentShaderName, WeakPointer<Graphics> graphics) : Material(graphics) {
        this->vertexShaderName = vertexShaderName;
        this->fragmentShaderName = fragmentShaderName;
        this->useBuiltInShader = false;
    }

    ShaderMaterial::ShaderMaterial(const std::string& builtInShaderName, WeakPointer<Graphics> graphics) : Material(graphics) {
        this->builtInShaderName = builtInShaderName;
        this->useBuiltInShader = true;
    }

    Bool ShaderMaterial::build() {
        WeakPointer<Graphics> graphics = Engine::instance()->getGraphicsSystem();
        ShaderManager& shaderManager = graphics->getShaderManager();
        Bool ready = false;
        if (this->useBuiltInShader) {
           this->shader = shaderManager.getShader(this->builtInShaderName);
           if (this->shader.isValid()) ready = true;
        } else {
            const std::string& vertexSrc = shaderManager.getShaderSource(ShaderType::Vertex, this->vertexShaderName);
            const std::string& fragmentSrc = shaderManager.getShaderSource(ShaderType::Fragment, this->fragmentShaderName);
            ready = this->buildFromSource(vertexSrc, fragmentSrc);
        }
        if (!ready) {
            return false;
        }
        graphics->activateShader(this->shader);
        this->bindShaderVarLocations();
        return true;
    }

    void ShaderMaterial::copyTo(WeakPointer<Material> target) {
        Material::copyTo(target);
    }
}
