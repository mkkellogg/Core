#include <string>
#include <unordered_map>

#include "StandardUniforms.h"

namespace Core {
/*

   ModelMatrix = 0,
        ViewMatrix = 1,
        ModelViewMatrix = 2,
        ProjectionMatrix = 3,
        ModelViewProjectionMatrix = 4,
        Texture0 = 5,
        Texture1 = 6,
         LightPosition = 7,
        LightDirection = 8,
        LightColor = 9,
        LightIntensity = 10,
        LightAttenuation = 11,
        LightType = 12,
        LightRange = 13,
        LightEnabled = 14,
        _Count = 15  // Must always be last in the list
        */

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
        "LIGHT_SHADOW_CUBE_MAP",
        "LIGHT_SHADOW_BIAS"
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
        {uniformNames[(UInt16)StandardUniform::LightEnabled],StandardUniform::LightShadowBias},
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