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
        LightPosition = 7,
        LightDirection = 8,
        LightColor = 9,
        LightIntensity = 10,
        LightAttenuation = 11,
        LightType = 12,
        LightRange = 13,
        LightEnabled = 14,
        LightMatrix = 15,
        LightShadowMap = 16,
        LightShadowCubeMap = 17,
        LightConstantShadowBias = 18,
        LightAngularShadowBias = 19,
        LightShadowMapSize = 20,
        LightShadowMapAspect = 21,
        LightShadowSoftness = 22,
        LightViewProjection = 23,
        LightCascadeEnd = 24,
        LightCascadeCount = 25,
        LightNearPlane = 26,
        LightIrradianceMap = 27,
        LightSpecularIBLPreFilteredMap = 28,
        LightSpecularIBLBRDFMap = 29,
        LightCount = 30,
        AmbientLightCount = 31,
        AmbientIBLLightCount = 32,
        PointLightCount = 33,
        DirectionalLightCount = 34,
        CameraPosition = 35,
        Texture0 = 36,
        DepthTexture = 37,
        _Count = 38,  // Must always be last in the list (before _None)
        _None = 39,
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
