#pragma once

#include "../util/WeakPointer.h"
#include "../render/ToneMapType.h"
#include "ShaderMaterial.h"
#include "BaseMaterial.h"

namespace Core {

    // forward declarations
    class Engine;
    class Texture2D;

    class TonemapMaterial : public ShaderMaterial<BaseMaterial> {
        friend class Engine;

    public:
        virtual ~TonemapMaterial();
        virtual Int32 getShaderLocation(StandardUniform uniform, UInt32 offset = 0) override;
        virtual void sendCustomUniformsToShader() override;
        virtual void copyTo(WeakPointer<Material> target) override;
        virtual WeakPointer<Material> clone() override;
        virtual void bindShaderVarLocations() override;
        virtual UInt32 textureCount() override;

        void setExposure(Real exposure);
        void setGamma(Real gamma);
        void setTexture(WeakPointer<Texture2D> texture);
        void setDepthTexture(WeakPointer<Texture2D> depthTexture);
        void setToneMapType(ToneMapType type);
        
    protected:
        TonemapMaterial(WeakPointer<Graphics> graphics);
        TonemapMaterial(const std::string& builtInShaderName, WeakPointer<Graphics> graphics);
        TonemapMaterial(const std::string& vertShaderName, const std::string& fragShaderName, WeakPointer<Graphics> graphics);

        Real exposure;
        Real gamma;
        PersistentWeakPointer<Texture2D> texture;
        PersistentWeakPointer<Texture2D> depthTexture;
        ToneMapType toneMapType;

        Int32 textureLocation;
        Int32 depthTextureLocation;
        Int32 exposureLocation;
        Int32 gammaLocation;
        Int32 toneMapTypeLocation;

    private:
        void setInitialParams();
    };
}
