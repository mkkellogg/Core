#pragma once

#include <unordered_map>
#include <memory>
#include <vector>

#include "../common/types.h"

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
        LightShadowCubeMap = 20,
        LightConstantShadowBias = 21,
        LightAngularShadowBias = 22,
        LightShadowMapSize = 23,
        LightShadowMapAspect = 24,
        LightShadowSoftness = 25,
        LightViewProjection = 26,
        LightCascadeEnd = 27,
        LightCascadeCount = 28,
        LightNearPlane = 29,
        _Count = 30,  // Must always be last in the list (before _None)
        _None = 31,
    };

    class StandardUniforms {
    public:
        static const std::string& getUniformName(StandardUniform uniform);
        static StandardUniform getUniformForName(const std::string& name);
    
    private:
        StandardUniforms();
        void init();
        static std::shared_ptr<StandardUniforms> instance;
        static void checkAndInitInstance();

        const std::string& _getUniformName(StandardUniform uniform);
        StandardUniform _getUniformForName(const std::string& name);
        
        std::vector<std::string> uniformNames;
        std::unordered_map<std::string, StandardUniform> nameToUniform;
    };
}
