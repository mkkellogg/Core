#pragma once

#include "../../util/WeakPointer.h"
#include "../../common/Constants.h"
#include "../../material/Material.h"
#include "../../color/Color.h"

namespace Core {

    // forward declarations
    class Engine;
    class Texture;

    class ParticleStandardMaterial: public Material {
        friend class Engine;

    public:
        virtual Bool build() override;
        virtual Int32 getShaderLocation(StandardAttribute attribute, UInt32 offset = 0);
        virtual Int32 getShaderLocation(StandardUniform uniform, UInt32 offset = 0);
        Int32 getWorldPositionLocation();
        Int32 getSizeLocation();
        Int32 getRotationLocation();
        virtual void copyTo(WeakPointer<Material> target) override;
        virtual WeakPointer<Material> clone() override;
        void bindShaderVarLocations();

    private:

        ParticleStandardMaterial();

        Int32 worldPositionLocation;
        Int32 sizeLocation;
        Int32 rotationLocation;
        Int32 projectionMatrixLocation;
        Int32 viewMatrixLocation;

    };
}
