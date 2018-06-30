#pragma once

#include "../common/types.h"
#include <unordered_map>

namespace Core {

    enum class StandardUniform {
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
        _Count = 15,  // Must always be last in the list (before _None)
        _None = 16,
    };

    class StandardUniforms {
        static const std::string uniformNames[];
        static std::unordered_map<std::string, StandardUniform> nameToUniform;

    public:
        static const std::string& getUniformName(StandardUniform uniform);
        static StandardUniform getUniformForName(const std::string& name);

    };
}
