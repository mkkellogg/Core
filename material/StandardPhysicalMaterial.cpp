#include "StandardPhysicalMaterial.h"
#include "../material/Shader.h"
#include "../util/WeakPointer.h"
#include "StandardAttributes.h"
#include "StandardUniforms.h"
#include "../image/Texture.h"
#include "../Engine.h"
#include "../material/ShaderManager.h"

namespace Core {

    StandardPhysicalMaterial::StandardPhysicalMaterial(const std::string& vertexShader, const std::string& fragmentShader, WeakPointer<Graphics> graphics):  ShaderMaterial(vertexShader, fragmentShader, graphics)  {
        this->albedo.set(1.0f, 1.0f, 1.0f, 1.0f);
        this->metallic = 0.9f;
        this->roughness = 0.35f;
        this->ambientOcclusion = 1.0f;

        this->albedoMapEnabled = false;
        this->normalMapEnabled = false;
        this->roughnessMapEnabled = false;
    }

    StandardPhysicalMaterial::StandardPhysicalMaterial(WeakPointer<Graphics> graphics): StandardPhysicalMaterial("StandardPhysical", "StandardPhysical", graphics) {
       
    }

    Bool StandardPhysicalMaterial::build() {
        ShaderMaterial::build();
        this->setLit(true);
        this->setPhysical(true);
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
            case StandardUniform::LightIrradianceMap:
                return this->lightIrradianceMapLocation;
            case StandardUniform::LightSpecularIBLPreFilteredMap:
                return this->lightSpecularIBLPreFilteredMapLocation;
            case StandardUniform::LightSpecularIBLBRDFMap:
                return this->lightSpecularIBLBRDFMapLocation;
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

    void StandardPhysicalMaterial::setAlbedo(Color albedo) {
        this->albedo = albedo;
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
        if (this->albedoMapEnabled) {
            this->shader->setTexture2D(textureLoc, this->albedoMap->getTextureID());
            this->shader->setUniform1i(this->albedoMapLocation, textureLoc);
            textureLoc++;
        }
        else {
            this->shader->setUniform4f(this->albedoLocation, this->albedo.r, this->albedo.g, this->albedo.b, this->albedo.a);
        }
        if (this->normalMapEnabled) {
            this->shader->setTexture2D(textureLoc, this->normalMap->getTextureID());
            this->shader->setUniform1i(this->normalMapLocation, textureLoc);
            textureLoc++;
        }
        this->shader->setUniform1f(this->metallicLocation, this->metallic);
        if (this->roughnessMapEnabled) {
            this->shader->setTexture2D(textureLoc, this->roughnessMap->getTextureID());
            this->shader->setUniform1i(this->roughnessLocation, textureLoc);
            textureLoc++;
        }
        else {
            this->shader->setUniform1f(this->roughnessLocation, this->roughness);
        }
        this->shader->setUniform1f(this->ambientOcclusionLocation, this->ambientOcclusion);
        this->shader->setUniform1i(this->enabledMapLocation, this->getEnabledMapMask());
    }

    void StandardPhysicalMaterial::copyTo(WeakPointer<Material> target) {
        WeakPointer<StandardPhysicalMaterial> targetMaterial = WeakPointer<Material>::dynamicPointerCast<StandardPhysicalMaterial>(target);
        ShaderMaterial::copyTo(targetMaterial);
        targetMaterial->albedoMap = this->albedoMap;
        targetMaterial->normalMap = this->normalMap;
        targetMaterial->roughnessMap = this->roughnessMap;
        targetMaterial->albedoMapEnabled = this->albedoMapEnabled;
        targetMaterial->normalMapEnabled = this->normalMapEnabled;
        targetMaterial->roughnessMapEnabled = this->roughnessMapEnabled;
        targetMaterial->positionLocation = this->positionLocation;
        targetMaterial->normalLocation = this->normalLocation;
        targetMaterial->faceNormalLocation = this->faceNormalLocation;
        targetMaterial->tangentLocation = this->tangentLocation;
        targetMaterial->colorLocation = this->colorLocation;
        targetMaterial->albedoUVLocation = this->albedoUVLocation;
        targetMaterial->normalUVLocation = this->normalUVLocation;
        targetMaterial->projectionMatrixLocation = this->projectionMatrixLocation;
        targetMaterial->viewMatrixLocation = this->viewMatrixLocation;
        targetMaterial->modelMatrixLocation = this->modelMatrixLocation;
        targetMaterial->modelInverseTransposeMatrixLocation = this->modelInverseTransposeMatrixLocation;
        targetMaterial->albedoLocation = this->albedoLocation;
        targetMaterial->albedoMapLocation = this->albedoMapLocation;
        targetMaterial->normalMapLocation = this->normalMapLocation;
        targetMaterial->roughnessMapLocation = this->roughnessMapLocation;
        targetMaterial->lightPositionLocation = this->lightPositionLocation;
        targetMaterial->lightDirectionLocation = this->lightDirectionLocation;
        targetMaterial->lightRangeLocation = this->lightRangeLocation;
        targetMaterial->lightTypeLocation = this->lightTypeLocation;
        targetMaterial->lightIntensityLocation = this->lightIntensityLocation;
        targetMaterial->lightColorLocation = this->lightColorLocation;
        targetMaterial->lightEnabledLocation = this->lightEnabledLocation;
        targetMaterial->lightMatrixLocation = this->lightMatrixLocation;
        for (UInt32 i =0; i < Constants::MaxDirectionalCascades; i++) {
            targetMaterial->lightViewProjectionLocations[i] = this->lightViewProjectionLocations[i];
            targetMaterial->lightShadowMapLocations[i] = this->lightShadowMapLocations[i];
            targetMaterial->lightCascadeEndLocations[i] = this->lightCascadeEndLocations[i];
            targetMaterial->lightShadowMapAspectLocations[i] = this->lightShadowMapAspectLocations[i];
        }
        targetMaterial->lightCascadeCountLocation = this->lightCascadeCountLocation;
        targetMaterial->lightShadowCubeMapLocation = this->lightShadowCubeMapLocation;
        targetMaterial->lightAngularShadowBiasLocation = this->lightAngularShadowBiasLocation;
        targetMaterial->lightConstantShadowBiasLocation = this->lightConstantShadowBiasLocation;
        targetMaterial->lightShadowMapSizeLocation = this->lightShadowMapSizeLocation;
        targetMaterial->lightShadowSoftnessLocation = this->lightShadowSoftnessLocation;
        targetMaterial->lightNearPlaneLocation = this->lightNearPlaneLocation;
        targetMaterial->lightIrradianceMapLocation = this->lightIrradianceMapLocation;
        targetMaterial->lightSpecularIBLPreFilteredMapLocation = this->lightSpecularIBLPreFilteredMapLocation;
        targetMaterial->lightSpecularIBLBRDFMapLocation = this->lightSpecularIBLBRDFMapLocation;
        targetMaterial->lightCountLocation = this->lightCountLocation;
        targetMaterial->cameraPositionLocation = this->cameraPositionLocation;
        targetMaterial->metallicLocation = this->metallicLocation;
        targetMaterial->roughnessLocation = this->roughnessLocation;
        targetMaterial->ambientOcclusionLocation = this->ambientOcclusionLocation;
        targetMaterial->enabledMapLocation = this->enabledMapLocation;
        targetMaterial->metallic = this->metallic;
        targetMaterial->roughness = this->roughness;
        targetMaterial->ambientOcclusion = this->ambientOcclusion;
    }

    WeakPointer<Material> StandardPhysicalMaterial::clone() {
        WeakPointer<StandardPhysicalMaterial> newMaterial = Engine::instance()->createMaterial<StandardPhysicalMaterial>(false);
        this->copyTo(newMaterial);
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
        this->albedoLocation = this->shader->getUniformLocation("albedo");
        this->albedoMapLocation = this->shader->getUniformLocation("albedoMap");
        this->normalMapLocation = this->shader->getUniformLocation("normalMap");
        this->roughnessMapLocation = this->shader->getUniformLocation("roughnessMap");
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

        this->lightIrradianceMapLocation = this->shader->getUniformLocation(StandardUniform::LightIrradianceMap);
        this->lightSpecularIBLPreFilteredMapLocation = this->shader->getUniformLocation(StandardUniform::LightSpecularIBLPreFilteredMap);
        this->lightSpecularIBLBRDFMapLocation = this->shader->getUniformLocation(StandardUniform::LightSpecularIBLBRDFMap);

        this->lightCountLocation = this->shader->getUniformLocation(StandardUniform::LightCount);

        this->cameraPositionLocation = this->shader->getUniformLocation(StandardUniform::CameraPosition);
        this->metallicLocation = this->shader->getUniformLocation("metallic");
        this->roughnessLocation = this->shader->getUniformLocation("roughness");
        this->ambientOcclusionLocation = this->shader->getUniformLocation("ambientOcclusion");
        this->enabledMapLocation = this->shader->getUniformLocation("enabledMap");
    }

    UInt32 StandardPhysicalMaterial::textureCount() {
        UInt32 textureCount = 0;
        if (this->albedoMapEnabled) textureCount++;
        if (this->normalMapEnabled) textureCount++;
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