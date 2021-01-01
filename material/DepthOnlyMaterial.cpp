#include "DepthOnlyMaterial.h"
#include "../material/Shader.h"
#include "../util/WeakPointer.h"
#include "StandardAttributes.h"
#include "StandardUniforms.h"
#include "../Engine.h"
#include "../Graphics.h"
#include "../material/ShaderManager.h"

namespace Core {

    DepthOnlyMaterial::DepthOnlyMaterial() {
    }

    DepthOnlyMaterial::~DepthOnlyMaterial() {
    }

    Bool DepthOnlyMaterial::build() {
        WeakPointer<Graphics> graphics = Engine::instance()->getGraphicsSystem();
        ShaderManager& shaderManager = graphics->getShaderManager();
        this->shader = shaderManager.getShader("Depth");
        graphics->activateShader(this->shader);
        this->bindShaderVarLocations();
        return true;
    }

    WeakPointer<Material> DepthOnlyMaterial::clone() {
        WeakPointer<DepthOnlyMaterial> newMaterial = Engine::instance()->createMaterial<DepthOnlyMaterial>(false);
        this->copyTo(newMaterial);
        return newMaterial;
    }
}