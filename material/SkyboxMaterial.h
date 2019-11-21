#pragma once

#include "../util/WeakPointer.h"
#include "ShaderMaterial.h"

namespace Core {

    // forward declarations
    class Engine;
    class CubeTexture;

    class SkyboxMaterial : public ShaderMaterial {
        friend class Engine;

    public:
        virtual Bool build() override;
        virtual Int32 getShaderLocation(StandardAttribute attribute, UInt32 offset = 0) override;
        virtual Int32 getShaderLocation(StandardUniform uniform, UInt32 offset = 0) override;
        virtual void sendCustomUniformsToShader() override;
        virtual WeakPointer<Material> clone() override;
        void setTexture(WeakPointer<CubeTexture> texture);
        virtual UInt32 textureCount() override;

    protected:
        SkyboxMaterial(WeakPointer<Graphics> graphics);
        SkyboxMaterial(const std::string& builtInShaderName, WeakPointer<Graphics> graphics);
        SkyboxMaterial(const std::string& vertShaderName, const std::string& fragShaderName, WeakPointer<Graphics> graphics);
        virtual void copyTo(WeakPointer<Material> target) override;
        virtual void bindShaderVarLocations() override;
        
        Int32 positionLocation;
        Int32 colorLocation;
        Int32 projectionMatrixLocation;
        Int32 viewMatrixLocation;
        Int32 cubeTextureLocation;
        PersistentWeakPointer<CubeTexture> texture;
    };
}
