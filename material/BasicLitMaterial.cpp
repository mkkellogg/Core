#include "BasicLitMaterial.h"
#include "../material/Shader.h"
#include "../util/WeakPointer.h"
#include "../image/Texture.h"
#include "../Engine.h"
#include "../Graphics.h"
#include "../material/ShaderManager.h"

namespace Core {

    BasicLitMaterial::BasicLitMaterial(WeakPointer<Graphics> graphics): BaseLitMaterial(graphics) {
    }

    Bool BasicLitMaterial::build() {
        WeakPointer<Graphics> graphics = Engine::instance()->getGraphicsSystem();
        ShaderManager& shaderManager = graphics->getShaderManager();
        this->shader = shaderManager.getShader("BasicLit");
        graphics->activateShader(this->shader);
        this->bindShaderVarLocations();
        this->setLit(true);
        return true;
    }

    WeakPointer<Material> BasicLitMaterial::clone() {
        WeakPointer<BasicLitMaterial> newMaterial = Engine::instance()->createMaterial<BasicLitMaterial>(false);
        this->copyTo(newMaterial);
        return newMaterial;
    }
}