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
        "TEXTURE0",
        "TEXTURE1",
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
        "LIGHT_VIEW_PROJECTION",
        "LIGHT_CASCADE_END",
        "LIGHT_CASCADE_COUNT",
        "LIGHT_SHADOW_CUBE_MAP",
        "LIGHT_CONSTANT_SHADOW_BIAS",
        "LIGHT_ANGULAR_SHADOW_BIAS",
        "LIGHT_SHADOW_MAP_SIZE",
        "LIGHT_SHADOW_MAP_ASPECT",
        "LIGHT_SHADOW_SOFTNESS"
    };

    std::unordered_map<std::string, StandardUniform> StandardUniforms::nameToUniform
    {
        {uniformNames[(UInt16)StandardUniform::ModelMatrix],StandardUniform::ModelMatrix},
        {uniformNames[(UInt16)StandardUniform::ViewMatrix],StandardUniform::ViewMatrix},
        {uniformNames[(UInt16)StandardUniform::ModelViewMatrix],StandardUniform::ModelViewMatrix},
        {uniformNames[(UInt16)StandardUniform::ProjectionMatrix],StandardUniform::ProjectionMatrix},
        {uniformNames[(UInt16)StandardUniform::ModelViewProjectionMatrix],StandardUniform::ModelViewProjectionMatrix},
        {uniformNames[(UInt16)StandardUniform::Texture0],StandardUniform::Texture0},
        {uniformNames[(UInt16)StandardUniform::Texture1],StandardUniform::Texture1},
        {uniformNames[(UInt16)StandardUniform::LightPosition],StandardUniform::LightPosition},
        {uniformNames[(UInt16)StandardUniform::LightDirection],StandardUniform::LightDirection},
        {uniformNames[(UInt16)StandardUniform::LightColor],StandardUniform::LightColor},
        {uniformNames[(UInt16)StandardUniform::LightIntensity],StandardUniform::LightIntensity},
        {uniformNames[(UInt16)StandardUniform::LightAttenuation],StandardUniform::LightAttenuation},
        {uniformNames[(UInt16)StandardUniform::LightRange],StandardUniform::LightRange},
        {uniformNames[(UInt16)StandardUniform::LightEnabled],StandardUniform::LightEnabled},
        {uniformNames[(UInt16)StandardUniform::LightEnabled],StandardUniform::LightMatrix},
        {uniformNames[(UInt16)StandardUniform::LightEnabled],StandardUniform::LightShadowMap},
        {uniformNames[(UInt16)StandardUniform::LightEnabled],StandardUniform::LightShadowCubeMap},
        {uniformNames[(UInt16)StandardUniform::LightEnabled],StandardUniform::LightConstantShadowBias},
        {uniformNames[(UInt16)StandardUniform::LightEnabled],StandardUniform::LightAngularShadowBias},
        {uniformNames[(UInt16)StandardUniform::LightEnabled],StandardUniform::LightShadowMapSize},
        {uniformNames[(UInt16)StandardUniform::LightEnabled],StandardUniform::LightShadowMapAspect},
        {uniformNames[(UInt16)StandardUniform::LightEnabled],StandardUniform::LightShadowSoftness}
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