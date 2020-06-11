#include "StandardPhysicalMaterialMultiLight.h"
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

    StandardPhysicalMaterialMultiLight::StandardPhysicalMaterialMultiLight(WeakPointer<Graphics> graphics): StandardPhysicalMaterial("StandardPhysicalMulti", graphics) {
        this->setRenderPath(RenderPath::SinglePassMultiLight);
    }

    UInt32 StandardPhysicalMaterialMultiLight::maxLightCount() const {
        return 2;
    }

    void StandardPhysicalMaterialMultiLight::copyAttributesFromStandardPhysicalMaterial(WeakPointer<StandardPhysicalMaterial> source) {

        this->albedo.set(source->albedo.r, source->albedo.g, source->albedo.b, source->albedo.a);
        this->metallic = source->metallic;
        this->roughness = source->roughness;
        this->ambientOcclusion = source->ambientOcclusion;

        this->albedoMapEnabled = source->albedoMapEnabled;
        this->normalMapEnabled = source->normalMapEnabled;
        this->roughnessMapEnabled = source->roughnessMapEnabled;
        this->metallicMapEnabled = source->metallicMapEnabled;

        this->albedoMap = source->albedoMap;
        this->normalMap = source->normalMap;
        this->roughnessMap = source->roughnessMap;
        this->metallicMap = source->metallicMap;
    }
}