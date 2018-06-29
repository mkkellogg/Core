#include "BasicCubeMaterial.h"
#include "../material/Shader.h"
#include "../util/WeakPointer.h"
#include "StandardAttributes.h"
#include "StandardUniforms.h"
#include "../Engine.h"
#include "../image/CubeTexture.h"
#include "../material/ShaderDirectory.h"

namespace Core {

    BasicCubeMaterial::BasicCubeMaterial(WeakPointer<Graphics> graphics) : Material(graphics) {
    }

    Bool BasicCubeMaterial::build() {
        WeakPointer<Graphics> graphics = Engine::instance()->getGraphicsSystem();
        ShaderDirectory& shaderDirectory = graphics->getShaderDirectory();
        const std::string& vertexSrc = shaderDirectory.getShader(Shader::ShaderType::Vertex, "BasicCube");
        const std::string& fragmentSrc = shaderDirectory.getShader(Shader::ShaderType::Fragment, "BasicCube");
        Bool ready = this->buildFromSource(vertexSrc, fragmentSrc);
        if (!ready) {
            return false;
        }
        this->bindShaderVarLocations();
        return true;
    }

    Int32 BasicCubeMaterial::getShaderLocation(StandardAttribute attribute) {
        switch (attribute) {
            case StandardAttribute::Position:
                return this->positionLocation;
            case StandardAttribute::Color:
                return this->colorLocation;
            default:
                return -1;
        }
    }

    Int32 BasicCubeMaterial::getShaderLocation(StandardUniform uniform) {
        switch (uniform) {
            case StandardUniform::ProjectionMatrix:
                return this->projectionMatrixLocation;
            case StandardUniform::ViewMatrix:
                return this->viewMatrixLocation;
            default:
                return -1;
        }
    }

    void BasicCubeMaterial::setTexture(WeakPointer<CubeTexture> texture) {
        this->texture = texture;
    }

    void BasicCubeMaterial::sendCustomUniformsToShader() {
        this->shader->setTextureCube(0, this->texture->getTextureID());
        this->shader->setUniform1i(textureLocation, 0);
    }

    WeakPointer<Material> BasicCubeMaterial::clone() {
        WeakPointer<BasicCubeMaterial> newMaterial = Engine::instance()->createMaterial<BasicCubeMaterial>(false);
        newMaterial->setShader(this->getShader());
        newMaterial->positionLocation = this->positionLocation;
        newMaterial->colorLocation = this->colorLocation;
        newMaterial->projectionMatrixLocation = this->projectionMatrixLocation;
        newMaterial->viewMatrixLocation = this->viewMatrixLocation;
        newMaterial->textureLocation = this->textureLocation;
        return newMaterial;
    }

    void BasicCubeMaterial::bindShaderVarLocations() {
        this->positionLocation = this->shader->getAttributeLocation("pos");
        this->colorLocation = this->shader->getAttributeLocation("color");
        this->textureLocation = this->shader->getUniformLocation("skybox");
        this->projectionMatrixLocation = this->shader->getUniformLocation("projection");
        this->viewMatrixLocation = this->shader->getUniformLocation("viewMatrix");
    }
}
