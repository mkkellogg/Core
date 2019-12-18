#include "BasicCubeMaterial.h"
#include "../material/Shader.h"
#include "../util/WeakPointer.h"
#include "../Engine.h"
#include "../Graphics.h"
#include "../image/CubeTexture.h"
#include "../image/Texture2D.h"
#include "../material/ShaderManager.h"

namespace Core {

    BasicCubeMaterial::BasicCubeMaterial(WeakPointer<Graphics> graphics) : BaseMaterial(graphics) {
        this->cubeTextureLocation = -1;
        this->rectTextureLocation = -1;
    }

    BasicCubeMaterial::~BasicCubeMaterial() {
        if (this->cubeTexture.isValid()) Graphics::safeReleaseObject(this->cubeTexture);
        if (this->rectTexture.isValid()) Graphics::safeReleaseObject(this->rectTexture);
    }

    Bool BasicCubeMaterial::build() {
        WeakPointer<Graphics> graphics = Engine::instance()->getGraphicsSystem();
        ShaderManager& shaderManager = graphics->getShaderManager();
        this->shader = shaderManager.getShader("BasicCube");
        graphics->activateShader(this->shader);
        this->bindShaderVarLocations();
        return true;
    }

    void BasicCubeMaterial::setCubeTexture(WeakPointer<CubeTexture> texture) {
        this->cubeTexture = texture;
    }

    void BasicCubeMaterial::setRectTexture(WeakPointer<Texture2D> texture) {
        this->rectTexture = texture;
    }

    void BasicCubeMaterial::sendCustomUniformsToShader() {
        this->shader->setTextureCube(0, this->cubeTexture->getTextureID());
        this->shader->setUniform1i(this->cubeTextureLocation, 0);
        this->shader->setTexture2D(1, this->rectTexture->getTextureID());
        this->shader->setUniform1i(this->rectTextureLocation, 1);
    }

    void BasicCubeMaterial::copyTo(WeakPointer<Material> target) {
        WeakPointer<BasicCubeMaterial> basicCubeMaterial = WeakPointer<Material>::dynamicPointerCast<BasicCubeMaterial>(target);
        if (basicCubeMaterial.isValid()) {
            BaseMaterial::copyTo(target);
            basicCubeMaterial->cubeTextureLocation = this->cubeTextureLocation;
            basicCubeMaterial->rectTextureLocation = this->rectTextureLocation;
            basicCubeMaterial->rectTexture = this->rectTexture;
            basicCubeMaterial->cubeTexture = this->cubeTexture;
        } else {
            throw InvalidArgumentException("BasicCubeMaterial::copyTo() -> 'target must be same material.");
        }
    }

    WeakPointer<Material> BasicCubeMaterial::clone() {
        WeakPointer<BasicCubeMaterial> newMaterial = Engine::instance()->createMaterial<BasicCubeMaterial>(false);
        this->copyTo(newMaterial);
        return newMaterial;
    }

    void BasicCubeMaterial::bindShaderVarLocations() {
        BaseMaterial::bindShaderVarLocations();
        this->cubeTextureLocation = this->shader->getUniformLocation("cubeTexture");
        this->rectTextureLocation = this->shader->getUniformLocation("rectTexture");
    }

    UInt32 BasicCubeMaterial::textureCount() {
        return 1;
    }
}
