#include "IrradianceRendererMaterial.h"
#include "../material/Shader.h"
#include "../util/WeakPointer.h"
#include "StandardAttributes.h"
#include "StandardUniforms.h"
#include "../Engine.h"
#include "../material/ShaderManager.h"

namespace Core {

    IrradianceRendererMaterial::IrradianceRendererMaterial(WeakPointer<Graphics> graphics) : SkyboxMaterial("IrradianceRenderer", "IrradianceRenderer", graphics) {
        this->setPhysical(true);
    }
}
