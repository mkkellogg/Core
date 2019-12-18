#include "BasicMaterial.h"
#include "../material/Shader.h"
#include "../util/WeakPointer.h"
#include "StandardAttributes.h"
#include "StandardUniforms.h"
#include "../Engine.h"
#include "../Graphics.h"
#include "../material/ShaderManager.h"

namespace Core {

    BasicMaterial::BasicMaterial(WeakPointer<Graphics> graphics) : BaseMaterial(graphics) {
    }

    Bool BasicMaterial::build() {
        WeakPointer<Graphics> graphics = Engine::instance()->getGraphicsSystem();
        ShaderManager& shaderManager = graphics->getShaderManager();
        this->shader = shaderManager.getShader("Basic");
        graphics->activateShader(this->shader);
        this->bindShaderVarLocations();
        return true;
    }

    WeakPointer<Material> BasicMaterial::clone() {
        WeakPointer<BasicMaterial> newMaterial = Engine::instance()->createMaterial<BasicMaterial>(false);
        this->copyTo(newMaterial);
        return newMaterial;
    }
}