#include "BasicTexturedLitMaterial.h"
#include "../material/Shader.h"
#include "../util/WeakPointer.h"
#include "StandardAttributes.h"
#include "StandardUniforms.h"
#include "../image/Texture.h"
#include "../Engine.h"
#include "../material/ShaderManager.h"

namespace Core {

    BasicTexturedLitMaterial::BasicTexturedLitMaterial(WeakPointer<Graphics> graphics): Material(graphics) {
    }

    Bool BasicTexturedLitMaterial::build() {
        WeakPointer<Graphics> graphics = Engine::instance()->getGraphicsSystem();
        ShaderManager& shaderDirectory = graphics->getShaderManager();
        const std::string& vertexSrc = shaderDirectory.getShader(ShaderType::Vertex, "BasicTexturedLit");
        const std::string& fragmentSrc = shaderDirectory.getShader(ShaderType::Fragment, "BasicTexturedLit");
        Bool ready = this->buildFromSource(vertexSrc, fragmentSrc);
        if (!ready) {
            return false;
        }
        this->bindShaderVarLocations();
        return true;
    }

    Int32 BasicTexturedLitMaterial::getShaderLocation(StandardAttribute attribute, UInt32 offset) {
        switch (attribute) {
            case StandardAttribute::Position:
                return this->positionLocation;
            case StandardAttribute::Normal:
                return this->normalLocation;
            case StandardAttribute::FaceNormal:
                return this->faceNormalLocation;
            case StandardAttribute::Color:
                return this->colorLocation;
            case StandardAttribute::UV0:
                return this->uvLocation;
            default:
                return -1;
        }
    }

    Int32 BasicTexturedLitMaterial::getShaderLocation(StandardUniform uniform, UInt32 offset) {
        if (offset >= Constants::MaxDirectionalCascades) {
            throw InvalidArgumentException("BasicLitMaterial::getShaderLocation() -> invalid offset.");
        }

        switch (uniform) {
            case StandardUniform::ProjectionMatrix:
                return this->projectionMatrixLocation;
            case StandardUniform::ViewMatrix:
                return this->viewMatrixLocation;
            case StandardUniform::ModelMatrix:
                return this->modelMatrixLocation;
            case StandardUniform::ModelInverseTransposeMatrix:
                return this->modelInverseTransposeMatrixLocation;
            case StandardUniform::Texture2D0:
                return this->textureLocation;
            case StandardUniform::LightPosition:
                return this->lightPositionLocation;
            case StandardUniform::LightDirection:
                return this->lightDirectionLocation;
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
            case StandardUniform::LightViewProjection:
                return this->lightViewProjectionLocations[offset];
            case StandardUniform::LightShadowMap:
                return this->lightShadowMapLocations[offset];
            case StandardUniform::LightCascadeEnd:
                return this->lightCascadeEndLocations[offset];
            case StandardUniform::LightShadowMapAspect:
                return this->lightShadowMapAspectLocations[offset];
            case StandardUniform::LightCascadeCount:
                return this->lightCascadeCountLocation;
            case StandardUniform::LightShadowCubeMap:
                return this->lightShadowCubeMapLocation;
            case StandardUniform::LightAngularShadowBias:
                return this->lightAngularShadowBiasLocation;
            case StandardUniform::LightConstantShadowBias:
                return this->lightConstantShadowBiasLocation;
            case StandardUniform::LightShadowMapSize:
                return this->lightShadowMapSizeLocation;
            case StandardUniform::LightShadowSoftness:
                return this->lightShadowSoftnessLocation;
            case StandardUniform::LightNearPlane:
                return this->lightNearPlaneLocation;
            default:
                return -1;
        }
    }

    void BasicTexturedLitMaterial::setTexture(WeakPointer<Texture> texture) {
        this->texture = texture;
    }

    void BasicTexturedLitMaterial::sendCustomUniformsToShader() {
        if (this->texture) {
            this->shader->setTexture2D(0, this->texture->getTextureID());
            this->shader->setUniform1i(textureLocation, 0);
        }
    }

    WeakPointer<Material> BasicTexturedLitMaterial::clone() {
        WeakPointer<BasicTexturedLitMaterial> newMaterial = Engine::instance()->createMaterial<BasicTexturedLitMaterial>(false);
        newMaterial->setShader(this->getShader());
        newMaterial->texture = this->texture;
        newMaterial->positionLocation = this->positionLocation;
        newMaterial->normalLocation = this->normalLocation;
        newMaterial->faceNormalLocation = this->faceNormalLocation;
        newMaterial->colorLocation = this->colorLocation;
        newMaterial->projectionMatrixLocation = this->projectionMatrixLocation;
        newMaterial->viewMatrixLocation = this->viewMatrixLocation;
        newMaterial->modelMatrixLocation = this->modelMatrixLocation;
        newMaterial->modelInverseTransposeMatrixLocation = this->modelInverseTransposeMatrixLocation;
        newMaterial->uvLocation = this->uvLocation;
        newMaterial->textureLocation = this->textureLocation;
        newMaterial->lightPositionLocation = this->lightPositionLocation;
        newMaterial->lightDirectionLocation = this->lightDirectionLocation;
        newMaterial->lightRangeLocation = this->lightRangeLocation;
        newMaterial->lightTypeLocation = this->lightTypeLocation;
        newMaterial->lightIntensityLocation = this->lightIntensityLocation;
        newMaterial->lightColorLocation = this->lightColorLocation;
        newMaterial->lightEnabledLocation = this->lightEnabledLocation;
        newMaterial->lightMatrixLocation = this->lightMatrixLocation;
        for (UInt32 i =0; i < Constants::MaxDirectionalCascades; i++) {
            newMaterial->lightViewProjectionLocations[i] = this->lightViewProjectionLocations[i];
            newMaterial->lightShadowMapLocations[i] = this->lightShadowMapLocations[i];
            newMaterial->lightCascadeEndLocations[i] = this->lightCascadeEndLocations[i];
            newMaterial->lightShadowMapAspectLocations[i] = this->lightShadowMapAspectLocations[i];
        }
        newMaterial->lightCascadeCountLocation = this->lightCascadeCountLocation;
        newMaterial->lightShadowCubeMapLocation = this->lightShadowCubeMapLocation;
        newMaterial->lightAngularShadowBiasLocation = this->lightAngularShadowBiasLocation;
        newMaterial->lightConstantShadowBiasLocation = this->lightConstantShadowBiasLocation;
        newMaterial->lightShadowMapSizeLocation = this->lightShadowMapSizeLocation;
        newMaterial->lightShadowSoftnessLocation = this->lightShadowSoftnessLocation;
        newMaterial->lightNearPlaneLocation = this->lightNearPlaneLocation;
        return newMaterial;
    }

    void BasicTexturedLitMaterial::bindShaderVarLocations() {
        this->positionLocation = this->shader->getAttributeLocation(StandardAttribute::Position);
        this->normalLocation = this->shader->getAttributeLocation(StandardAttribute::Normal);
        this->faceNormalLocation = this->shader->getAttributeLocation(StandardAttribute::FaceNormal);
        this->colorLocation = this->shader->getAttributeLocation(StandardAttribute::Color);
        this->textureLocation = this->shader->getUniformLocation(StandardUniform::Texture2D0);
        this->uvLocation = this->shader->getAttributeLocation(StandardAttribute::UV0);
        this->projectionMatrixLocation = this->shader->getUniformLocation(StandardUniform::ProjectionMatrix);
        this->viewMatrixLocation = this->shader->getUniformLocation(StandardUniform::ViewMatrix);
        this->modelMatrixLocation = this->shader->getUniformLocation(StandardUniform::ModelMatrix);
        this->modelInverseTransposeMatrixLocation = this->shader->getUniformLocation(StandardUniform::ModelInverseTransposeMatrix);
        this->lightPositionLocation = this->shader->getUniformLocation(StandardUniform::LightPosition);
        this->lightDirectionLocation = this->shader->getUniformLocation(StandardUniform::LightDirection);
        this->lightRangeLocation = this->shader->getUniformLocation(StandardUniform::LightRange);
        this->lightTypeLocation = this->shader->getUniformLocation(StandardUniform::LightType);
        this->lightIntensityLocation = this->shader->getUniformLocation(StandardUniform::LightIntensity);
        this->lightColorLocation = this->shader->getUniformLocation(StandardUniform::LightColor);
        this->lightEnabledLocation = this->shader->getUniformLocation(StandardUniform::LightEnabled);
        this->lightMatrixLocation = this->shader->getUniformLocation(StandardUniform::LightMatrix);
        for (UInt32 i =0; i < Constants::MaxDirectionalCascades; i++) {
            this->lightViewProjectionLocations[i] = this->shader->getUniformLocation(StandardUniform::LightViewProjection, i);
            this->lightShadowMapLocations[i] = this->shader->getUniformLocation(StandardUniform::LightShadowMap, i);
            this->lightCascadeEndLocations[i] = this->shader->getUniformLocation(StandardUniform::LightCascadeEnd, i);
            this->lightShadowMapAspectLocations[i] = this->shader->getUniformLocation(StandardUniform::LightShadowMapAspect, i);
        }
        this->lightCascadeCountLocation = this->shader->getUniformLocation(StandardUniform::LightCascadeCount);
        this->lightShadowCubeMapLocation = this->shader->getUniformLocation(StandardUniform::LightShadowCubeMap);
        this->lightAngularShadowBiasLocation = this->shader->getUniformLocation(StandardUniform::LightAngularShadowBias);
        this->lightConstantShadowBiasLocation = this->shader->getUniformLocation(StandardUniform::LightConstantShadowBias);
        this->lightShadowMapSizeLocation = this->shader->getUniformLocation(StandardUniform::LightShadowMapSize);
        this->lightShadowSoftnessLocation = this->shader->getUniformLocation(StandardUniform::LightShadowSoftness);
        this->lightNearPlaneLocation = this->shader->getUniformLocation(StandardUniform::LightNearPlane);
    }

    UInt32 BasicTexturedLitMaterial::textureCount() {
        return 1;
    }
}