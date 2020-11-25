#include "DistanceOnlyMaterial.h"
#include "../material/Shader.h"
#include "../Engine.h"
#include "../Graphics.h"
#include "../material/ShaderManager.h"

namespace Core {

    DistanceOnlyMaterial::DistanceOnlyMaterial() {
    }

    DistanceOnlyMaterial::~DistanceOnlyMaterial() {
    }

    Bool DistanceOnlyMaterial::build() {
        WeakPointer<Graphics> graphics = Engine::instance()->getGraphicsSystem();
        ShaderManager& shaderManager = graphics->getShaderManager();
        this->shader = shaderManager.getShader("Distance");
        graphics->activateShader(this->shader);
        this->bindShaderVarLocations();
        this->setSkinningEnabled(true);
        return true;
    }

    WeakPointer<Material> DistanceOnlyMaterial::clone() {
        WeakPointer<DistanceOnlyMaterial> newMaterial = Engine::instance()->createMaterial<DistanceOnlyMaterial>(false);
        this->copyTo(newMaterial);
        return newMaterial;
    }
}