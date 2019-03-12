#pragma once

#include "../util/WeakPointer.h"
#include "ShaderMaterial.h"

namespace Core {

    // forward declarations
    class Engine;
    class Texture2D;

    class TonemapMaterial : public ShaderMaterial {
        friend class Engine;

    public:
        virtual Bool build() override;
        virtual Int32 getShaderLocation(StandardAttribute attribute, UInt32 offset = 0) override;
        virtual Int32 getShaderLocation(StandardUniform uniform, UInt32 offset = 0) override;
        virtual void sendCustomUniformsToShader() override;
        virtual WeakPointer<Material> clone() override;
        void setExposure(Real exposure);
        void setTexture(WeakPointer<Texture2D> texture);
        void setDepthTexture(WeakPointer<Texture2D> depthTexture);
        virtual UInt32 textureCount() override;
        
    protected:
        TonemapMaterial(WeakPointer<Graphics> graphics);
        TonemapMaterial(const std::string& vertShaderName, const std::string& fragShaderName, WeakPointer<Graphics> graphics);
        virtual void copyTo(WeakPointer<Material> target) override;
        virtual void bindShaderVarLocations() override;

        Real exposure;
        PersistentWeakPointer<Texture2D> texture;
        PersistentWeakPointer<Texture2D> depthTexture;

        Int32 positionLocation;
        Int32 colorLocation;
        Int32 projectionMatrixLocation;
        Int32 viewMatrixLocation;
        Int32 modelMatrixLocation;
        Int32 textureLocation;
        Int32 depthTextureLocation;
        Int32 exposureLocation;
    };
}
