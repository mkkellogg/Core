#include "StandardPhysicalMaterialMultiLight.h"
#include "../material/Shader.h"
#include "StandardAttributes.h"
#include "StandardUniforms.h"
#include "../image/Texture.h"
#include "../image/Texture2D.h"
#include "../image/CubeTexture.h"
#include "../Engine.h"
#include "../Graphics.h"
#include "../material/ShaderManager.h"

namespace Core {

    StandardPhysicalMaterialMultiLight::StandardPhysicalMaterialMultiLight(WeakPointer<Graphics> graphics): StandardPhysicalMaterial("StandardPhysicalMulti", graphics) {
        this->setRenderPath(RenderPath::SinglePassMultiLight);
    }

    UInt32 StandardPhysicalMaterialMultiLight::maxLightCount() const {
        return 4;
    }
}