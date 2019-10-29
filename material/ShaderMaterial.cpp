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
    }

    Bool ShaderMaterial::build() {
        WeakPointer<Graphics> graphics = Engine::instance()->getGraphicsSystem();
        ShaderManager& shaderDirectory = graphics->getShaderManager();
        const std::string& vertexSrc = shaderDirectory.getShader(ShaderType::Vertex, this->vertexShaderName);
        const std::string& fragmentSrc = shaderDirectory.getShader(ShaderType::Fragment, this->fragmentShaderName);
        Bool ready = this->buildFromSource(vertexSrc, fragmentSrc);
        this->bindShaderVarLocations();
        if (!ready) {
            return false;
        }
        return true;
    }

    void ShaderMaterial::copyTo(WeakPointer<Material> target) {
        Material::copyTo(target);
    }
}
