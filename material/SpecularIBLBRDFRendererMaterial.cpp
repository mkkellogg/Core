#include "SpecularIBLBRDFRendererMaterial.h"
#include "../material/Shader.h"
#include "../util/WeakPointer.h"
#include "../Engine.h"

namespace Core {

    SpecularIBLBRDFRendererMaterial::SpecularIBLBRDFRendererMaterial(const std::string& vertShaderName, const std::string& fragShaderName):
        ShaderMaterial<BaseMaterial>(vertShaderName, fragShaderName) {
    }
    
    SpecularIBLBRDFRendererMaterial::SpecularIBLBRDFRendererMaterial():
        ShaderMaterial<BaseMaterial>("SpecularIBLBRDFRenderer") {
    }

    SpecularIBLBRDFRendererMaterial::~SpecularIBLBRDFRendererMaterial() {
    }

    WeakPointer<Material> SpecularIBLBRDFRendererMaterial::clone() {
        WeakPointer<SpecularIBLBRDFRendererMaterial> newMaterial = Engine::instance()->createMaterial<SpecularIBLBRDFRendererMaterial>(false);
        this->copyTo(newMaterial);
        return newMaterial;
    }
}
