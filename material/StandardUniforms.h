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
        LightShadowsEnabled = 15,
        LightMatrix = 16,
        LightShadowMap = 17,
        LightShadowCubeMap = 18,
        LightConstantShadowBias = 19,
        LightAngularShadowBias = 20,
        LightShadowMapSize = 21,
        LightShadowMapAspect = 22,
        LightShadowSoftness = 23,
        LightViewProjection = 24,
        LightCascadeEnd = 25,
        LightCascadeCount = 26,
        LightNearPlane = 27,
        LightIrradianceMap = 28,
        LightSpecularIBLPreFilteredMap = 29,
        LightSpecularIBLBRDFMap = 30,
        LightCount = 31,
        AmbientLightCount = 32,
        AmbientIBLLightCount = 33,
        PointLightCount = 34,
        DirectionalLightCount = 35,
        CameraPosition = 36,
        Texture0 = 37,
        DepthTexture = 38,
        SkinningEnabled = 39,
        Bones = 40,
        SSAOMap = 41,
        SSAOEnabled = 42,
        _Count = 43,  // Must always be last in the list (before _None)
        _None = 44,
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
