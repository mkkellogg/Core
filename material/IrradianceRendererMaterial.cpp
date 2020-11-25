#include "IrradianceRendererMaterial.h"

namespace Core {

    IrradianceRendererMaterial::IrradianceRendererMaterial() : SkyboxMaterial("IrradianceRenderer") {
        this->setPhysical(true);
    }
}
