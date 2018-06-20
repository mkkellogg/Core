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
        _Count = 7  // Must always be last in the list
        */

    const std::string StandardUniforms::uniformNames[] =
    {
        "MODEL_MATRIX",
        "VIEW_MATRIX",
        "MODELVIEW_MATRIX",
        "PROJECTION_MATRIX",
        "MODELVIEWPROJECTION_MATRIX",
        "TEXTURE0",
        "TEXTURE1"
    };

    std::unordered_map<std::string, StandardUniform> StandardUniforms::nameToUniform
    {
        {uniformNames[(UInt16)StandardUniform::ModelMatrix],StandardUniform::ModelMatrix},
        {uniformNames[(UInt16)StandardUniform::ViewMatrix],StandardUniform::ViewMatrix},
        {uniformNames[(UInt16)StandardUniform::ModelViewMatrix],StandardUniform::ModelViewMatrix},
        {uniformNames[(UInt16)StandardUniform::ProjectionMatrix],StandardUniform::ProjectionMatrix},
        {uniformNames[(UInt16)StandardUniform::ModelViewProjectionMatrix],StandardUniform::ModelViewProjectionMatrix},
        {uniformNames[(UInt16)StandardUniform::Texture0],StandardUniform::Texture0},
        {uniformNames[(UInt16)StandardUniform::Texture1],StandardUniform::Texture1}
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