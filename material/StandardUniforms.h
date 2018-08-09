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
        Texture2D0 = 7,
        Texture2D1 = 8,
        CubeTexture0 = 9,
        LightPosition = 10,
        LightDirection = 11,
        LightColor = 12,
        LightIntensity = 13,
        LightAttenuation = 14,
        LightType = 15,
        LightRange = 16,
        LightEnabled = 17,
        LightMatrix = 18,
        LightShadowMap = 19,
        LightViewProjection = 20,
        LightCascadeEnd = 21,
        LightCascadeCount = 22,
        LightShadowCubeMap = 23,
        LightConstantShadowBias = 24,
        LightAngularShadowBias = 25,
        LightShadowMapSize = 26,
        LightShadowMapAspect = 27,
        LightShadowSoftness = 28,
        _Count = 29,  // Must always be last in the list (before _None)
        _None = 30,
    };

    class StandardUniforms {
        static const std::string uniformNames[];
        static std::unordered_map<std::string, StandardUniform> nameToUniform;

    public:
        static const std::string& getUniformName(StandardUniform uniform);
        static StandardUniform getUniformForName(const std::string& name);

    };
}
