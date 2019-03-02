#include "StandardPhysicalMaterial.h"
#include "../material/Shader.h"
#include "../util/WeakPointer.h"
#include "StandardAttributes.h"
#include "StandardUniforms.h"
#include "../image/Texture.h"
#include "../Engine.h"
#include "../material/ShaderManager.h"

namespace Core {

    StandardPhysicalMaterial::StandardPhysicalMaterial(WeakPointer<Graphics> graphics): Material(graphics) {
        this->metallic = 0.5f;
        this->roughness = 0.1f;
        this->ambientOcclusion = 0.0f;

        this->albedoMapEnabled = false;
        this->normalMapEnabled = false;
        this->roughnessMapEnabled = false;
    }

    Bool StandardPhysicalMaterial::build() {
        WeakPointer<Graphics> graphics = Engine::instance()->getGraphicsSystem();
        ShaderManager& shaderDirectory = graphics->getShaderManager();
        const std::string& vertexSrc = shaderDirectory.getShader(ShaderType::Vertex, "StandardPhysical");
        const std::string& fragmentSrc = shaderDirectory.getShader(ShaderType::Fragment, "StandardPhysical");
        Bool ready = this->buildFromSource(vertexSrc, fragmentSrc);
        if (!ready) {
            return false;
        }
        this->bindShaderVarLocations();
        this->lit = true;
        return true;
    }

    Int32 StandardPhysicalMaterial::getShaderLocation(StandardAttribute attribute, UInt32 offset) {
        switch (attribute) {
            case StandardAttribute::Position:
                return this->positionLocation;
            case StandardAttribute::Normal:
                return this->normalLocation;
            case StandardAttribute::FaceNormal:
                return this->faceNormalLocation;
            case StandardAttribute::Tangent:
                return this->tangentLocation;
            case StandardAttribute::Color:
                return this->colorLocation;
            case StandardAttribute::AlbedoUV:
                return this->albedoUVLocation;
             case StandardAttribute::NormalUV:
                return this->normalUVLocation;
            default:
                return -1;
        }
    }

    Int32 StandardPhysicalMaterial::getShaderLocation(StandardUniform uniform, UInt32 offset) {
        if (offset >= Constants::MaxDirectionalCascades) {
            throw InvalidArgumentException("StandardPhysicalMaterial::getShaderLocation() -> invalid offset.");
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
            case StandardUniform::LightCount:
                return this->lightCountLocation;
            case StandardUniform::CameraPosition:
                return this->cameraPositionLocation;
            default:
                return -1;
        }
    }

    void StandardPhysicalMaterial::setMetallic(Real metallic) {
        this->metallic = metallic;
    }

    void StandardPhysicalMaterial::setRoughness(Real roughness) {
        this->roughness = roughness;
    }

    void StandardPhysicalMaterial::setAmbientOcclusion(Real ambientOcclusion) {
        this->ambientOcclusion = ambientOcclusion;
    }

    void StandardPhysicalMaterial::setAlbedoMap(WeakPointer<Texture> albedoMap) {
        this->albedoMap = albedoMap;
    }

    void StandardPhysicalMaterial::setNormalMap(WeakPointer<Texture> normalMap) {
        this->normalMap = normalMap;
    }

    void StandardPhysicalMaterial::setRoughnessMap(WeakPointer<Texture> roughnessMap) {
        this->roughnessMap = roughnessMap;
    }

    void StandardPhysicalMaterial::setAlbedoMapEnabled(Bool enabled) {
        this->albedoMapEnabled = enabled;
    }

    void StandardPhysicalMaterial::setNormalMapEnabled(Bool enabled) {
        this->normalMapEnabled = enabled;
    }

    void StandardPhysicalMaterial::setRoughnessMapEnabled(Bool enabled) {
        this->roughnessMapEnabled = enabled;
    }

    void StandardPhysicalMaterial::sendCustomUniformsToShader() {
        UInt32 textureLoc = 0;
        if (this->albedoMap) {
            this->shader->setTexture2D(textureLoc, this->albedoMap->getTextureID());
            this->shader->setUniform1i(this->albedoMapLocation, textureLoc);
            textureLoc++;
        }
        if (this->normalMap) {
            this->shader->setTexture2D(textureLoc, this->normalMap->getTextureID());
            this->shader->setUniform1i(this->normalMapLocation, textureLoc);
            textureLoc++;
        }
        this->shader->setUniform1f(this->metallicLocation, this->metallic);
        this->shader->setUniform1f(this->roughnessLocation, this->roughness);
        this->shader->setUniform1f(this->ambientOcclusion, this->ambientOcclusion);
        this->shader->setUniform1i(this->enabledMapLcation, this->getEnabledMapMask());
    }

    WeakPointer<Material> StandardPhysicalMaterial::clone() {
        WeakPointer<StandardPhysicalMaterial> newMaterial = Engine::instance()->createMaterial<StandardPhysicalMaterial>(false);
        this->copyTo(newMaterial);
        newMaterial->albedoMap = this->albedoMap;
        newMaterial->positionLocation = this->positionLocation;
        newMaterial->normalLocation = this->normalLocation;
        newMaterial->faceNormalLocation = this->faceNormalLocation;
        newMaterial->tangentLocation = this->tangentLocation;
        newMaterial->colorLocation = this->colorLocation;
        newMaterial->albedoUVLocation = this->albedoUVLocation;
        newMaterial->normalUVLocation = this->normalUVLocation;
        newMaterial->projectionMatrixLocation = this->projectionMatrixLocation;
        newMaterial->viewMatrixLocation = this->viewMatrixLocation;
        newMaterial->modelMatrixLocation = this->modelMatrixLocation;
        newMaterial->modelInverseTransposeMatrixLocation = this->modelInverseTransposeMatrixLocation;
        newMaterial->albedoMapLocation = this->albedoMapLocation;
        newMaterial->normalMapLocation = this->normalMapLocation;
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
        newMaterial->lightCountLocation = this->lightCountLocation;
        newMaterial->cameraPositionLocation = this->cameraPositionLocation;
        newMaterial->metallicLocation = this->metallicLocation;
        newMaterial->roughnessLocation = this->roughnessLocation;
        newMaterial->ambientOcclusionLocation = this->ambientOcclusionLocation;
        newMaterial->enabledMapLcation = this->enabledMapLcation;
        return newMaterial;
    }

    void StandardPhysicalMaterial::bindShaderVarLocations() {
        this->positionLocation = this->shader->getAttributeLocation(StandardAttribute::Position);
        this->normalLocation = this->shader->getAttributeLocation(StandardAttribute::Normal);
        this->faceNormalLocation = this->shader->getAttributeLocation(StandardAttribute::FaceNormal);
        this->tangentLocation = this->shader->getAttributeLocation(StandardAttribute::Tangent);
        this->colorLocation = this->shader->getAttributeLocation(StandardAttribute::Color);
        this->albedoUVLocation = this->shader->getAttributeLocation(StandardAttribute::AlbedoUV);
        this->normalUVLocation = this->shader->getAttributeLocation(StandardAttribute::NormalUV);
        this->albedoMapLocation = this->shader->getUniformLocation("albedoMap");
        this->normalMapLocation = this->shader->getUniformLocation("normalMap");
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
        this->lightCountLocation = this->shader->getUniformLocation(StandardUniform::LightCount);

        this->cameraPositionLocation = this->shader->getUniformLocation(StandardUniform::CameraPosition);
        this->metallicLocation = this->shader->getUniformLocation("metallic");
        this->roughnessLocation = this->shader->getUniformLocation("roughness");
        this->ambientOcclusionLocation = this->shader->getUniformLocation("ambientOcclusion");
        this->enabledMapLcation = this->shader->getUniformLocation("enabledMap");
    }

    UInt32 StandardPhysicalMaterial::textureCount() {
        UInt32 textureCount = 0;
        if (this->albedoMap) textureCount++;
        if (this->normalMap) textureCount++;
       return textureCount;
    }

    UInt32 StandardPhysicalMaterial::getEnabledMapMask() {
        UInt32 mask = 0;
        if (this->albedoMapEnabled) mask = mask | ALBEDO_MAP_MASK;
        if (this->normalMapEnabled) mask = mask | NORMAL_MAP_MASK;
        if (this->roughnessMapEnabled) mask = mask | ROUGHNESS_MAP_MASK;
        return mask;
    }
}