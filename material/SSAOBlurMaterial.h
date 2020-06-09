#pragma once

#include <vector>

#include "BaseMaterial.h"
#include "../util/WeakPointer.h"
#include "../math/Matrix4x4.h"
#include "../geometry/Vector3.h"

namespace Core {

    // forward declarations
    class Engine;
    class Texture;

    class SSAOBlurMaterial : public BaseMaterial {
        friend class Engine;

    public:
        virtual ~SSAOBlurMaterial();
        virtual Bool build() override;
        virtual Int32 getShaderLocation(StandardAttribute attribute, UInt32 offset = 0) override;
        virtual void sendCustomUniformsToShader() override;
        virtual WeakPointer<Material> clone() override;
        virtual void copyTo(WeakPointer<Material> targetMaterial) override;
        virtual void bindShaderVarLocations() override;

        void setSSAOInput(WeakPointer<Texture> positions);

    private:
        SSAOBlurMaterial(WeakPointer<Graphics> graphics);

        Int32 ssaoInputLocation;
        Int32 albedoUVLocation;

        PersistentWeakPointer<Texture> ssaoInput;
    };
}
