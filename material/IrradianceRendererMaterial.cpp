#include "IrradianceRendererMaterial.h"

namespace Core {

    IrradianceRendererMaterial::IrradianceRendererMaterial(WeakPointer<Graphics> graphics) : SkyboxMaterial("IrradianceRenderer", graphics) {
        this->setPhysical(true);
    }
}
