#include "SpecularIBLBRDFRendererMaterial.h"
#include "../material/Shader.h"
#include "../util/WeakPointer.h"
#include "StandardAttributes.h"
#include "StandardUniforms.h"
#include "../Engine.h"
#include "../material/ShaderManager.h"

namespace Core {

    SpecularIBLBRDFRendererMaterial::SpecularIBLBRDFRendererMaterial(const std::string& vertShaderName, const std::string& fragShaderName, WeakPointer<Graphics> graphics):
        ShaderMaterial<BaseMaterial>(vertShaderName, fragShaderName, graphics) {
    }
    
    SpecularIBLBRDFRendererMaterial::SpecularIBLBRDFRendererMaterial(WeakPointer<Graphics> graphics):
        ShaderMaterial<BaseMaterial>("SpecularIBLBRDFRenderer", graphics) {
    }

    SpecularIBLBRDFRendererMaterial::~SpecularIBLBRDFRendererMaterial() {
    }

    WeakPointer<Material> SpecularIBLBRDFRendererMaterial::clone() {
        WeakPointer<SpecularIBLBRDFRendererMaterial> newMaterial = Engine::instance()->createMaterial<SpecularIBLBRDFRendererMaterial>(false);
        this->copyTo(newMaterial);
        return newMaterial;
    }
}
