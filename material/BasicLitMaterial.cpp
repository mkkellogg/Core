#include "BasicLitMaterial.h"
#include "../material/Shader.h"
#include "../util/WeakPointer.h"
#include "StandardAttributes.h"
#include "StandardUniforms.h"
#include "../image/Texture.h"
#include "../Engine.h"
#include "../material/ShaderManager.h"

namespace Core {

    BasicLitMaterial::BasicLitMaterial(WeakPointer<Graphics> graphics): Material(graphics) {
    }

    Bool BasicLitMaterial::build() {
        WeakPointer<Graphics> graphics = Engine::instance()->getGraphicsSystem();
        ShaderManager& shaderDirectory = graphics->getShaderManager();
        const std::string& vertexSrc = shaderDirectory.getShader(ShaderType::Vertex, "BasicLit");
        const std::string& fragmentSrc = shaderDirectory.getShader(ShaderType::Fragment, "BasicLit");
        Bool ready = this->buildFromSource(vertexSrc, fragmentSrc);
        if (!ready) {
            return false;
        }
        this->bindShaderVarLocations();
        return true;
    }

    Int32 BasicLitMaterial::getShaderLocation(StandardAttribute attribute) {
        switch (attribute) {
            case StandardAttribute::Position:
                return this->positionLocation;
             case StandardAttribute::Normal:
                return this->normalLocation;
            case StandardAttribute::Color:
                return this->colorLocation;
            case StandardAttribute::UV0:
                return this->uvLocation;
            default:
                return -1;
        }
    }

    Int32 BasicLitMaterial::getShaderLocation(StandardUniform uniform) {
        switch (uniform) {
            case StandardUniform::ProjectionMatrix:
                return this->projectionMatrixLocation;
            case StandardUniform::ViewMatrix:
                return this->viewMatrixLocation;
            case StandardUniform::ModelMatrix:
                return this->modelMatrixLocation;
             case StandardUniform::ModelInverseTransposeMatrix:
                return this->modelInverseTransposeMatrixLocation;
            case StandardUniform::LightPosition:
                return this->lightPositionLocation;
            case StandardUniform::LightRange:
                return this->lightRangeLocation;
            case StandardUniform::LightType:
                return this->lightTypeLocation;
            case StandardUniform::LightIntensity:
                return this->lightIntensityLocation;
            case StandardUniform::LightColor:
                return this->lightColorLocation;
            case StandardUniform::LightEnabled:
                return this->lightEnabledLocation;
            case StandardUniform::LightMatrix:
                return this->lightMatrixLocation;
            case StandardUniform::LightShadowMap:
                return this->lightShadowMapLocation;
            case StandardUniform::LightShadowCubeMap:
                return this->lightShadowCubeMapLocation;
            case StandardUniform::LightShadowBias:
                return this->lightShadowMapLocation;
            default:
                return -1;
        }
    }

    void BasicLitMaterial::sendCustomUniformsToShader() {

    }

    WeakPointer<Material> BasicLitMaterial::clone() {
        WeakPointer<BasicLitMaterial> newMaterial = Engine::instance()->createMaterial<BasicLitMaterial>(false);
        newMaterial->setShader(this->getShader());
        newMaterial->texture = this->texture;
        newMaterial->positionLocation = this->positionLocation;
        newMaterial->normalLocation = this->normalLocation;
        newMaterial->colorLocation = this->colorLocation;
        newMaterial->projectionMatrixLocation = this->projectionMatrixLocation;
        newMaterial->viewMatrixLocation = this->viewMatrixLocation;
        newMaterial->modelMatrixLocation = this->modelMatrixLocation;
        newMaterial->modelInverseTransposeMatrixLocation = this->modelInverseTransposeMatrixLocation;
        newMaterial->uvLocation = this->uvLocation;
        newMaterial->lightPositionLocation = this->lightPositionLocation;
        newMaterial->lightRangeLocation = this->lightRangeLocation;
        newMaterial->lightTypeLocation = this->lightTypeLocation;
        newMaterial->lightIntensityLocation = this->lightIntensityLocation;
        newMaterial->lightColorLocation = this->lightColorLocation;
        newMaterial->lightEnabledLocation = this->lightEnabledLocation;
        newMaterial->lightMatrixLocation = this->lightMatrixLocation;
        newMaterial->lightShadowMapLocation = this->lightShadowMapLocation;
        newMaterial->lightShadowCubeMapLocation = this->lightShadowCubeMapLocation;
        newMaterial->lightShadowMapLocation = this->lightShadowMapLocation;
        return newMaterial;
    }

    void BasicLitMaterial::bindShaderVarLocations() {
        this->positionLocation = this->shader->getAttributeLocation("pos");
        this->normalLocation = this->shader->getAttributeLocation("normal");
        this->colorLocation = this->shader->getAttributeLocation("color");
        this->uvLocation = this->shader->getAttributeLocation("uv");
        this->projectionMatrixLocation = this->shader->getUniformLocation("projection");
        this->viewMatrixLocation = this->shader->getUniformLocation("viewMatrix");
        this->modelMatrixLocation = this->shader->getUniformLocation("modelMatrix");
        this->modelInverseTransposeMatrixLocation = this->shader->getUniformLocation("modelInverseTransposeMatrix");
        this->lightPositionLocation = this->shader->getUniformLocation("lightPos");
        this->lightRangeLocation = this->shader->getUniformLocation("lightRange");
        this->lightTypeLocation = this->shader->getUniformLocation("lightType");
        this->lightIntensityLocation = this->shader->getUniformLocation("lightIntensity");
        this->lightColorLocation = this->shader->getUniformLocation("lightColor");
        this->lightEnabledLocation = this->shader->getUniformLocation("lightEnabled");
        this->lightMatrixLocation = this->shader->getUniformLocation("lightMatrix");
        this->lightShadowMapLocation = this->shader->getUniformLocation("lightShadowMap");
        this->lightShadowCubeMapLocation = this->shader->getUniformLocation("lightShadowCubeMap");
        this->lightShadowMapLocation = this->shader->getUniformLocation("lightShadowBias");
    }
}