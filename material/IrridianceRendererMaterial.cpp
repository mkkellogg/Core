#include "IrridianceRendererMaterial.h"
#include "../material/Shader.h"
#include "../util/WeakPointer.h"
#include "StandardAttributes.h"
#include "StandardUniforms.h"
#include "../Engine.h"
#include "../material/ShaderManager.h"

namespace Core {

    IrridianceRendererMaterial::IrridianceRendererMaterial(WeakPointer<Graphics> graphics) : SkyboxMaterial("IrridianceRenderer", "IrridianceRenderer", graphics) {

    }
}
