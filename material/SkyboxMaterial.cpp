#include "SkyboxMaterial.h"
#include "../material/Shader.h"
#include "../util/WeakPointer.h"
#include "StandardAttributes.h"
#include "StandardUniforms.h"
#include "../Engine.h"
#include "../image/CubeTexture.h"
#include "../material/ShaderManager.h"

namespace Core {

    SkyboxMaterial::SkyboxMaterial(WeakPointer<Graphics> graphics) : Material(graphics) {
    }

    Bool SkyboxMaterial::build() {
        WeakPointer<Graphics> graphics = Engine::instance()->getGraphicsSystem();
        ShaderManager& shaderDirectory = graphics->getShaderManager();
        const std::string& vertexSrc = shaderDirectory.getShader(ShaderType::Vertex, "Skybox");
        const std::string& fragmentSrc = shaderDirectory.getShader(ShaderType::Fragment, "Skybox");
        Bool ready = this->buildFromSource(vertexSrc, fragmentSrc);
        if (!ready) {
            return false;
        }
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
            case StandardUniform::CubeTexture0:
                return this->cubeTextureLocation;
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

    WeakPointer<Material> SkyboxMaterial::clone() {
        WeakPointer<SkyboxMaterial> newMaterial = Engine::instance()->createMaterial<SkyboxMaterial>(false);
        this->copyTo(newMaterial);
        newMaterial->positionLocation = this->positionLocation;
        newMaterial->colorLocation = this->colorLocation;
        newMaterial->projectionMatrixLocation = this->projectionMatrixLocation;
        newMaterial->viewMatrixLocation = this->viewMatrixLocation;
        newMaterial->cubeTextureLocation = this->cubeTextureLocation;
        return newMaterial;
    }

    void SkyboxMaterial::bindShaderVarLocations() {
        this->positionLocation = this->shader->getAttributeLocation(StandardAttribute::Position);
        this->colorLocation = this->shader->getAttributeLocation(StandardAttribute::Color);
        this->cubeTextureLocation = this->shader->getUniformLocation(StandardUniform::CubeTexture0);
        this->projectionMatrixLocation = this->shader->getUniformLocation(StandardUniform::ProjectionMatrix);
        this->viewMatrixLocation = this->shader->getUniformLocation(StandardUniform::ViewMatrix);
    }

    UInt32 SkyboxMaterial::textureCount() {
        return 1;
    }
}
