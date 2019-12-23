#include "StandardPhysicalMaterial.h"
#include "../material/Shader.h"
#include "StandardAttributes.h"
#include "StandardUniforms.h"
#include "../image/Texture.h"
#include "../image/Texture2D.h"
#include "../image/CubeTexture.h"
#include "../Engine.h"
#include "../Graphics.h"
#include "../material/ShaderManager.h"

namespace Core {

    StandardPhysicalMaterial::StandardPhysicalMaterial(const std::string& vertexShader, const std::string& fragmentShader, WeakPointer<Graphics> graphics):
        ShaderMaterial<BaseLitMaterial>(vertexShader, fragmentShader, graphics)  {
            this->setInitialParams();
    }

    StandardPhysicalMaterial::StandardPhysicalMaterial(const std::string& builtInShaderName, WeakPointer<Graphics> graphics):
        ShaderMaterial<BaseLitMaterial>(builtInShaderName, graphics) {
            this->setInitialParams();
    }

    StandardPhysicalMaterial::StandardPhysicalMaterial(WeakPointer<Graphics> graphics): StandardPhysicalMaterial("StandardPhysical", graphics) {
    }

    StandardPhysicalMaterial::~StandardPhysicalMaterial() {
        if (this->albedoMap.isValid()) Graphics::safeReleaseObject(this->albedoMap);
        if (this->normalMap.isValid()) Graphics::safeReleaseObject(this->normalMap);
        if (this->roughnessMap.isValid()) Graphics::safeReleaseObject(this->roughnessMap);
        if (this->metallicMap.isValid()) Graphics::safeReleaseObject(this->metallicMap);
    }

    void StandardPhysicalMaterial::setInitialParams() {
        this->albedo.set(1.0f, 1.0f, 1.0f, 1.0f);
        this->metallic = 0.9f;
        this->roughness = 0.35f;
        this->ambientOcclusion = 1.0f;

        this->albedoMapEnabled = false;
        this->normalMapEnabled = false;
        this->roughnessMapEnabled = false;
        this->metallicMapEnabled = false;

        this->albedoUVLocation = -1;
        this->normalUVLocation = -1;

        this->albedoLocation = -1;
        this->albedoMapLocation = -1;
        this->normalMapLocation = -1;
        this->roughnessMapLocation = -1;
        this->metallicMapLocation = -1;

        this->metallicLocation = -1;
        this->roughnessLocation = -1;
        this->ambientOcclusionLocation = -1;
        this->enabledMapLocation = -1;

        this->lightIrradianceMapLocation = -1;
        this->lightSpecularIBLPreFilteredMapLocation = -1;
        this->lightSpecularIBLBRDFMapLocation = -1;
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

    void StandardPhysicalMaterial::setMetallicMap(WeakPointer<Texture> metallicMap) {
        this->metallicMap = metallicMap;
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

    void StandardPhysicalMaterial::setMetallicMapEnabled(Bool enabled) {
        this->metallicMapEnabled = enabled;
    }

    Bool StandardPhysicalMaterial::build() {
        ShaderMaterial<BaseLitMaterial>::build();
        this->setLit(true);
        this->setPhysical(true);
        this->setSkinningEnabled(true);
        return true;
    }

    Int32 StandardPhysicalMaterial::getShaderLocation(StandardAttribute attribute, UInt32 offset) {
        Int32 attributeLocation = BaseMaterial::getShaderLocation(attribute, offset);
        if (attributeLocation >= 0) return attributeLocation;
        switch (attribute) {
            case StandardAttribute::AlbedoUV:
                return this->albedoUVLocation;
            case StandardAttribute::NormalUV:
                return this->normalUVLocation;
            default:
                return -1;
        }
    }

    Int32 StandardPhysicalMaterial::getShaderLocation(StandardUniform uniform, UInt32 offset) {
        Int32 uniformLocation = BaseLitMaterial::getShaderLocation(uniform, offset);
        if (uniformLocation >= 0) return uniformLocation;
        switch (uniform) {
            case StandardUniform::LightIrradianceMap:
                return this->lightIrradianceMapLocation;
            case StandardUniform::LightSpecularIBLBRDFMap:
                return this->lightSpecularIBLBRDFMapLocation;
            case StandardUniform::LightSpecularIBLPreFilteredMap:
                return this->lightSpecularIBLPreFilteredMapLocation;
            default:
                return -1;
        }
    }

    void StandardPhysicalMaterial::sendCustomUniformsToShader() {
        UInt32 textureLoc = 0;
        if (this->albedoMapEnabled) {
            this->shader->setTexture2D(textureLoc, this->albedoMap->getTextureID());
            this->shader->setUniform1i(this->albedoMapLocation, textureLoc);
        }
        else {
            this->shader->setUniform4f(this->albedoLocation, this->albedo.r, this->albedo.g, this->albedo.b, this->albedo.a);
            this->shader->setTexture2D(textureLoc, this->graphics->getPlaceHolderTexture2D()->getTextureID());
            this->shader->setUniform1i(this->albedoMapLocation, textureLoc);
        }
        textureLoc++;

        if (this->normalMapEnabled) {
            this->shader->setTexture2D(textureLoc, this->normalMap->getTextureID());
            this->shader->setUniform1i(this->normalMapLocation, textureLoc);
        } else {
            this->shader->setTexture2D(textureLoc, this->graphics->getPlaceHolderTexture2D()->getTextureID());
            this->shader->setUniform1i(this->normalMapLocation, textureLoc);
        }
        textureLoc++;

        if (this->metallicMapEnabled) {
            this->shader->setTexture2D(textureLoc, this->metallicMap->getTextureID());
            this->shader->setUniform1i(this->metallicMapLocation, textureLoc);
        }
        else {
            this->shader->setUniform1f(this->metallicLocation, this->metallic);
            this->shader->setTexture2D(textureLoc, this->graphics->getPlaceHolderTexture2D()->getTextureID());
            this->shader->setUniform1i(this->metallicMapLocation, textureLoc);
        }
        textureLoc++;

        if (this->roughnessMapEnabled) {
            this->shader->setTexture2D(textureLoc, this->roughnessMap->getTextureID());
            this->shader->setUniform1i(this->roughnessMapLocation, textureLoc);
        }
        else {
            this->shader->setUniform1f(this->roughnessLocation, this->roughness);
            this->shader->setTexture2D(textureLoc, this->graphics->getPlaceHolderTexture2D()->getTextureID());
            this->shader->setUniform1i(this->roughnessMapLocation, textureLoc);
        }
        textureLoc++;
        
        this->shader->setUniform1f(this->ambientOcclusionLocation, this->ambientOcclusion);
        this->shader->setUniform1i(this->enabledMapLocation, this->getEnabledMapMask());
    }

    void StandardPhysicalMaterial::copyTo(WeakPointer<Material> target) {
        WeakPointer<StandardPhysicalMaterial> standardPhysicalMaterial = WeakPointer<Material>::dynamicPointerCast<StandardPhysicalMaterial>(target);
        if (standardPhysicalMaterial.isValid()) {
            BaseLitMaterial::copyTo(standardPhysicalMaterial);
            standardPhysicalMaterial->albedo = this->albedo;
            standardPhysicalMaterial->metallic = this->metallic;
            standardPhysicalMaterial->roughness = this->roughness;
            standardPhysicalMaterial->ambientOcclusion = this->ambientOcclusion;

            standardPhysicalMaterial->albedoMap = this->albedoMap;
            standardPhysicalMaterial->normalMap = this->normalMap;
            standardPhysicalMaterial->roughnessMap = this->roughnessMap;
            standardPhysicalMaterial->metallicMap = this->metallicMap;
            
            standardPhysicalMaterial->albedoMapEnabled = this->albedoMapEnabled;
            standardPhysicalMaterial->normalMapEnabled = this->normalMapEnabled;
            standardPhysicalMaterial->roughnessMapEnabled = this->roughnessMapEnabled;
            standardPhysicalMaterial->metallicMapEnabled = this->metallicMapEnabled;

            standardPhysicalMaterial->albedoUVLocation = this->albedoUVLocation;
            standardPhysicalMaterial->normalUVLocation = this->normalUVLocation;
            standardPhysicalMaterial->albedoLocation = this->albedoLocation;
            standardPhysicalMaterial->albedoMapLocation = this->albedoMapLocation;
            standardPhysicalMaterial->normalMapLocation = this->normalMapLocation;
            standardPhysicalMaterial->roughnessMapLocation = this->roughnessMapLocation;
            standardPhysicalMaterial->metallicMapLocation = this->metallicMapLocation;
            standardPhysicalMaterial->metallicLocation = this->metallicLocation;
            standardPhysicalMaterial->roughnessLocation = this->roughnessLocation;
            standardPhysicalMaterial->ambientOcclusionLocation = this->ambientOcclusionLocation;
            standardPhysicalMaterial->enabledMapLocation = this->enabledMapLocation;
            standardPhysicalMaterial->lightIrradianceMapLocation = this->lightIrradianceMapLocation;
            standardPhysicalMaterial->lightSpecularIBLPreFilteredMapLocation = this->lightSpecularIBLPreFilteredMapLocation;
            standardPhysicalMaterial->lightSpecularIBLBRDFMapLocation = this->lightSpecularIBLBRDFMapLocation;
        } else {
            throw InvalidArgumentException("StandardPhysicalMaterial::copyTo() -> 'target must be same material."); 
        }
    }

    WeakPointer<Material> StandardPhysicalMaterial::clone() {
        WeakPointer<StandardPhysicalMaterial> newMaterial = Engine::instance()->createMaterial<StandardPhysicalMaterial>(false);
        this->copyTo(newMaterial);
        return newMaterial;
    }

    void StandardPhysicalMaterial::bindShaderVarLocations() {
        BaseLitMaterial::bindShaderVarLocations();
        this->lightIrradianceMapLocation = this->shader->getUniformLocation(StandardUniform::LightIrradianceMap);
        this->lightSpecularIBLPreFilteredMapLocation = this->shader->getUniformLocation(StandardUniform::LightSpecularIBLPreFilteredMap);
        this->lightSpecularIBLBRDFMapLocation = this->shader->getUniformLocation(StandardUniform::LightSpecularIBLBRDFMap);
        this->albedoUVLocation = this->shader->getAttributeLocation(StandardAttribute::AlbedoUV);
        this->normalUVLocation = this->shader->getAttributeLocation(StandardAttribute::NormalUV);
        this->albedoLocation = this->shader->getUniformLocation("albedo");
        this->albedoMapLocation = this->shader->getUniformLocation("albedoMap");
        this->normalMapLocation = this->shader->getUniformLocation("normalMap");
        this->roughnessMapLocation = this->shader->getUniformLocation("roughnessMap");
        this->metallicMapLocation = this->shader->getUniformLocation("metallicMap");
        this->metallicLocation = this->shader->getUniformLocation("metallic");
        this->roughnessLocation = this->shader->getUniformLocation("roughness");
        this->ambientOcclusionLocation = this->shader->getUniformLocation("ambientOcclusion");
        this->enabledMapLocation = this->shader->getUniformLocation("enabledMap");
    }

    UInt32 StandardPhysicalMaterial::textureCount() {
        return 4;
    }

    UInt32 StandardPhysicalMaterial::getEnabledMapMask() {
        UInt32 mask = 0;
        if (this->albedoMapEnabled) mask = mask | ALBEDO_MAP_MASK;
        if (this->normalMapEnabled) mask = mask | NORMAL_MAP_MASK;
        if (this->roughnessMapEnabled) mask = mask | ROUGHNESS_MAP_MASK;
        if (this->metallicMapEnabled) mask = mask | METALLIC_MAP_MASK;
        return mask;
    }
}