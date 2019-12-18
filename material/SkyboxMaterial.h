#pragma once

#include "../util/WeakPointer.h"
#include "ShaderMaterial.h"
#include "BaseMaterial.h"

namespace Core {

    // forward declarations
    class Engine;
    class CubeTexture;

    class SkyboxMaterial : public ShaderMaterial<BaseMaterial> {
        friend class Engine;

    public:
        virtual ~SkyboxMaterial();
        virtual void sendCustomUniformsToShader() override;
        virtual void copyTo(WeakPointer<Material> target) override;
        virtual WeakPointer<Material> clone() override;
        virtual void bindShaderVarLocations() override;
        virtual UInt32 textureCount() override;

        void setTexture(WeakPointer<CubeTexture> texture);

    protected:
        SkyboxMaterial(WeakPointer<Graphics> graphics);
        SkyboxMaterial(const std::string& builtInShaderName, WeakPointer<Graphics> graphics);
        SkyboxMaterial(const std::string& vertShaderName, const std::string& fragShaderName, WeakPointer<Graphics> graphics);
        
        Int32 cubeTextureLocation;
        PersistentWeakPointer<CubeTexture> texture;
    };
}
