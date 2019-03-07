#include "SkyboxMaterial.h"
#include "../material/Shader.h"
#include "../util/WeakPointer.h"
#include "StandardAttributes.h"
#include "StandardUniforms.h"
#include "../Engine.h"
#include "../image/CubeTexture.h"
#include "../material/ShaderManager.h"

namespace Core {

    SkyboxMaterial::SkyboxMaterial(const std::string& vertShaderName, const std::string& fragShaderName, WeakPointer<Graphics> graphics): ShaderMaterial(vertShaderName, fragShaderName, graphics) {

    }
    
    SkyboxMaterial::SkyboxMaterial(WeakPointer<Graphics> graphics) : SkyboxMaterial("Skybox", "Skybox", graphics) {
    }

    Bool SkyboxMaterial::build() {
        if(!ShaderMaterial::build()) return false;
        this->bindShaderVarLocations();
        return true;
    }

    Int32 SkyboxMaterial::getShaderLocation(StandardAttribute attribute, UInt32 offset) {
        switch (attribute) {
            case StandardAttribute::Position:
                return this->positionLocation;
            case StandardAttribute::Color:
                return this->colorLocation;
            default:
                return -1;
        }
    }

    Int32 SkyboxMaterial::getShaderLocation(StandardUniform uniform, UInt32 offset) {
        switch (uniform) {
            case StandardUniform::ProjectionMatrix:
                return this->projectionMatrixLocation;
            case StandardUniform::ViewMatrix:
                return this->viewMatrixLocation;
            case StandardUniform::ModelMatrix:
                return this->modelMatrixLocation;
            default:
                return -1;
        }
    }

    void SkyboxMaterial::setTexture(WeakPointer<CubeTexture> texture) {
        this->texture = texture;
    }

    void SkyboxMaterial::sendCustomUniformsToShader() {
        this->shader->setTextureCube(0, this->texture->getTextureID());
        this->shader->setUniform1i(this->cubeTextureLocation, 0);
    }

    void SkyboxMaterial::copyTo(WeakPointer<Material> target) {
        ShaderMaterial::copyTo(target);
        WeakPointer<SkyboxMaterial> _target = WeakPointer<Material>::dynamicPointerCast<SkyboxMaterial>(target);
        _target->positionLocation = this->positionLocation;
        _target->colorLocation = this->colorLocation;
        _target->projectionMatrixLocation = this->projectionMatrixLocation;
        _target->viewMatrixLocation = this->viewMatrixLocation;
        _target->modelMatrixLocation = this->modelMatrixLocation;
        _target->cubeTextureLocation = this->cubeTextureLocation;
    }

    WeakPointer<Material> SkyboxMaterial::clone() {
        WeakPointer<SkyboxMaterial> newMaterial = Engine::instance()->createMaterial<SkyboxMaterial>(false);
        this->copyTo(newMaterial);
        return newMaterial;
    }

    void SkyboxMaterial::bindShaderVarLocations() {
        this->positionLocation = this->shader->getAttributeLocation(StandardAttribute::Position);
        this->colorLocation = this->shader->getAttributeLocation(StandardAttribute::Color);
        this->cubeTextureLocation = this->shader->getUniformLocation("cubeTexture");
        this->projectionMatrixLocation = this->shader->getUniformLocation(StandardUniform::ProjectionMatrix);
        this->viewMatrixLocation = this->shader->getUniformLocation(StandardUniform::ViewMatrix);
        this->modelMatrixLocation = this->shader->getUniformLocation(StandardUniform::ModelMatrix);
    }

    UInt32 SkyboxMaterial::textureCount() {
        return 1;
    }
}
