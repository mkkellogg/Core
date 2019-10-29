#include "BasicCubeMaterial.h"
#include "../material/Shader.h"
#include "../util/WeakPointer.h"
#include "StandardAttributes.h"
#include "StandardUniforms.h"
#include "../Engine.h"
#include "../Graphics.h"
#include "../image/CubeTexture.h"
#include "../image/Texture2D.h"
#include "../material/ShaderManager.h"

namespace Core {

    BasicCubeMaterial::BasicCubeMaterial(WeakPointer<Graphics> graphics) : Material(graphics) {
    }

    Bool BasicCubeMaterial::build() {
        WeakPointer<Graphics> graphics = Engine::instance()->getGraphicsSystem();
        ShaderManager& shaderDirectory = graphics->getShaderManager();
        const std::string& vertexSrc = shaderDirectory.getShader(ShaderType::Vertex, "BasicCube");
        const std::string& fragmentSrc = shaderDirectory.getShader(ShaderType::Fragment, "BasicCube");
        Bool ready = this->buildFromSource(vertexSrc, fragmentSrc);
        if (!ready) {
            return false;
        }
        this->bindShaderVarLocations();
        return true;
    }

    Int32 BasicCubeMaterial::getShaderLocation(StandardAttribute attribute, UInt32 offset) {
        switch (attribute) {
            case StandardAttribute::Position:
                return this->positionLocation;
            case StandardAttribute::Color:
                return this->colorLocation;
            default:
                return -1;
        }
    }

    Int32 BasicCubeMaterial::getShaderLocation(StandardUniform uniform, UInt32 offset) {
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

    WeakPointer<Material> BasicCubeMaterial::clone() {
        WeakPointer<BasicCubeMaterial> newMaterial = Engine::instance()->createMaterial<BasicCubeMaterial>(false);
        this->copyTo(newMaterial);
        newMaterial->positionLocation = this->positionLocation;
        newMaterial->colorLocation = this->colorLocation;
        newMaterial->projectionMatrixLocation = this->projectionMatrixLocation;
        newMaterial->viewMatrixLocation = this->viewMatrixLocation;
        newMaterial->modelMatrixLocation = this->modelMatrixLocation;
        newMaterial->cubeTextureLocation = this->cubeTextureLocation;
        newMaterial->rectTextureLocation = this->rectTextureLocation;
        newMaterial->rectTexture = this->rectTexture;
        newMaterial->cubeTexture = this->cubeTexture;
        return newMaterial;
    }

    void BasicCubeMaterial::bindShaderVarLocations() {
        this->positionLocation = this->shader->getAttributeLocation(StandardAttribute::Position);
        this->colorLocation = this->shader->getAttributeLocation(StandardAttribute::Color);
        this->cubeTextureLocation = this->shader->getUniformLocation("cubeTexture");
        this->rectTextureLocation = this->shader->getUniformLocation("rectTexture");
        this->projectionMatrixLocation = this->shader->getUniformLocation(StandardUniform::ProjectionMatrix);
        this->viewMatrixLocation = this->shader->getUniformLocation(StandardUniform::ViewMatrix);
        this->modelMatrixLocation = this->shader->getUniformLocation(StandardUniform::ModelMatrix);
    }

    UInt32 BasicCubeMaterial::textureCount() {
        return 1;
    }
}
