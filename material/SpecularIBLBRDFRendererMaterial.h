#pragma once

#include "../util/WeakPointer.h"
#include "../render/ToneMapType.h"
#include "ShaderMaterial.h"
#include "BaseMaterial.h"

namespace Core {

    // forward declarations
    class Engine;
    class Texture2D;

    class SpecularIBLBRDFRendererMaterial : public ShaderMaterial<BaseMaterial> {
        friend class Engine;

    public:
        virtual ~SpecularIBLBRDFRendererMaterial();
        virtual WeakPointer<Material> clone() override;
        
    protected:
        SpecularIBLBRDFRendererMaterial();
        SpecularIBLBRDFRendererMaterial(const std::string& vertShaderName, const std::string& fragShaderName);
    };
}
