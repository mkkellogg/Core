#include <string>
#include <unordered_map>

#include "StandardUniforms.h"

namespace Core {

    const std::string StandardUniforms::uniformNames[] =
    {   
        "MODEL_MATRIX",
        "VIEW_MATRIX",
        "MODELVIEW_MATRIX",
        "PROJECTION_MATRIX",
        "MODELVIEWPROJECTION_MATRIX",
        "MODELINVERSETRANSPOSE_MATRIX",
        "VIEWINVERSETRANSPOSE_MATRIX",
        "TEXTURE2D0",
        "TEXTURE2D1",
        "CUBETEXTURE0",
        "LIGHT_POSITION",
        "LIGHT_DIRECTION",
        "LIGHT_COLOR",
        "LIGHT_INTENSITY",
        "LIGHT_ATTENUATION",
        "LIGHT_TYPE",
        "LIGHT_RANGE",
        "LIGHT_ENABLED",
        "LIGHT_MATRIX",
        "LIGHT_SHADOW_MAP",
        "LIGHT_SHADOW_CUBE_MAP",
        "LIGHT_CONSTANT_SHADOW_BIAS",
        "LIGHT_ANGULAR_SHADOW_BIAS",
        "LIGHT_SHADOW_MAP_SIZE",
        "LIGHT_SHADOW_MAP_ASPECT",
        "LIGHT_SHADOW_SOFTNESS",
        "LIGHT_VIEW_PROJECTION",
        "LIGHT_CASCADE_END",
        "LIGHT_CASCADE_COUNT",
        "LIGHT_NEAR_PLANE"
    };

    std::unordered_map<std::string, StandardUniform> StandardUniforms::nameToUniform
    {
        {uniformNames[(UInt16)StandardUniform::ModelMatrix],StandardUniform::ModelMatrix},
        {uniformNames[(UInt16)StandardUniform::ViewMatrix],StandardUniform::ViewMatrix},
        {uniformNames[(UInt16)StandardUniform::ModelViewMatrix],StandardUniform::ModelViewMatrix},
        {uniformNames[(UInt16)StandardUniform::ProjectionMatrix],StandardUniform::ProjectionMatrix},
        {uniformNames[(UInt16)StandardUniform::ModelViewProjectionMatrix],StandardUniform::ModelViewProjectionMatrix},
        {uniformNames[(UInt16)StandardUniform::ModelInverseTransposeMatrix],StandardUniform::ModelInverseTransposeMatrix},
        {uniformNames[(UInt16)StandardUniform::ViewInverseTransposeMatrix],StandardUniform::ViewInverseTransposeMatrix},
        {uniformNames[(UInt16)StandardUniform::Texture2D0],StandardUniform::Texture2D0},
        {uniformNames[(UInt16)StandardUniform::Texture2D1],StandardUniform::Texture2D1},
        {uniformNames[(UInt16)StandardUniform::CubeTexture0],StandardUniform::CubeTexture0},
        {uniformNames[(UInt16)StandardUniform::LightPosition],StandardUniform::LightPosition},
        {uniformNames[(UInt16)StandardUniform::LightDirection],StandardUniform::LightDirection},
        {uniformNames[(UInt16)StandardUniform::LightColor],StandardUniform::LightColor},
        {uniformNames[(UInt16)StandardUniform::LightIntensity],StandardUniform::LightIntensity},
        {uniformNames[(UInt16)StandardUniform::LightAttenuation],StandardUniform::LightAttenuation},
        {uniformNames[(UInt16)StandardUniform::LightType],StandardUniform::LightType},
        {uniformNames[(UInt16)StandardUniform::LightRange],StandardUniform::LightRange},
        {uniformNames[(UInt16)StandardUniform::LightEnabled],StandardUniform::LightEnabled},
        {uniformNames[(UInt16)StandardUniform::LightMatrix],StandardUniform::LightMatrix},
        {uniformNames[(UInt16)StandardUniform::LightShadowMap],StandardUniform::LightShadowMap},
        {uniformNames[(UInt16)StandardUniform::LightShadowCubeMap],StandardUniform::LightShadowCubeMap},
        {uniformNames[(UInt16)StandardUniform::LightConstantShadowBias],StandardUniform::LightConstantShadowBias},
        {uniformNames[(UInt16)StandardUniform::LightAngularShadowBias],StandardUniform::LightAngularShadowBias},
        {uniformNames[(UInt16)StandardUniform::LightShadowMapSize],StandardUniform::LightShadowMapSize},
        {uniformNames[(UInt16)StandardUniform::LightShadowMapAspect],StandardUniform::LightShadowMapAspect},
        {uniformNames[(UInt16)StandardUniform::LightShadowSoftness],StandardUniform::LightShadowSoftness},
        {uniformNames[(UInt16)StandardUniform::LightViewProjection],StandardUniform::LightViewProjection},
        {uniformNames[(UInt16)StandardUniform::LightCascadeEnd],StandardUniform::LightCascadeEnd},
        {uniformNames[(UInt16)StandardUniform::LightCascadeCount],StandardUniform::LightCascadeCount},
        {uniformNames[(UInt16)StandardUniform::LightNearPlane],StandardUniform::LightNearPlane}
    };

    const std::string& StandardUniforms::getUniformName(StandardUniform uniform) {
        return uniformNames[(UInt16)uniform];
    }

    StandardUniform StandardUniforms::getUniformForName(const std::string& name) {
        auto result = nameToUniform.find(name);
        if (result == nameToUniform.end()) {
            return StandardUniform::_None;
        }

        return (*result).second;
    }
}