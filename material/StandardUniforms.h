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
        ModelInverseTransposeMatrix = 5,
        ViewInverseTransposeMatrix = 6,
        Texture0 = 7,
        Texture1 = 8,
        LightPosition = 9,
        LightDirection = 10,
        LightColor = 11,
        LightIntensity = 12,
        LightAttenuation = 13,
        LightType = 14,
        LightRange = 15,
        LightEnabled = 16,
        LightMatrix = 17,
        LightShadowMap = 18,
        LightViewProjection = 19,
        LightCascadeEnd = 20,
        LightCascadeCount = 21,
        LightShadowCubeMap = 22,
        LightConstantShadowBias = 23,
        LightAngularShadowBias = 24,
        LightShadowMapSize = 25,
        _Count = 26,  // Must always be last in the list (before _None)
        _None = 27,
    };

    class StandardUniforms {
        static const std::string uniformNames[];
        static std::unordered_map<std::string, StandardUniform> nameToUniform;

    public:
        static const std::string& getUniformName(StandardUniform uniform);
        static StandardUniform getUniformForName(const std::string& name);

    };
}
