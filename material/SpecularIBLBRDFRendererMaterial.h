#pragma once

#include "../util/WeakPointer.h"
#include "../render/ToneMapType.h"
#include "ShaderMaterial.h"

namespace Core {

    // forward declarations
    class Engine;
    class Texture2D;

    class SpecularIBLBRDFRendererMaterial : public ShaderMaterial {
        friend class Engine;

    public:
        virtual Bool build() override;
        virtual Int32 getShaderLocation(StandardAttribute attribute, UInt32 offset = 0) override;
        virtual Int32 getShaderLocation(StandardUniform uniform, UInt32 offset = 0) override;
        virtual void sendCustomUniformsToShader() override;
        virtual WeakPointer<Material> clone() override;
        
    protected:
        SpecularIBLBRDFRendererMaterial(WeakPointer<Graphics> graphics);
        SpecularIBLBRDFRendererMaterial(const std::string& vertShaderName, const std::string& fragShaderName, WeakPointer<Graphics> graphics);
        virtual void copyTo(WeakPointer<Material> target) override;
        virtual void bindShaderVarLocations() override;

        Int32 positionLocation;
        Int32 projectionMatrixLocation;
        Int32 viewMatrixLocation;
        Int32 modelMatrixLocation;
    };
}
