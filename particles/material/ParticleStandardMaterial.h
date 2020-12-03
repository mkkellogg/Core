#pragma once

#include "../../util/WeakPointer.h"
#include "../../common/Constants.h"
#include "../../material/Material.h"
#include "../../color/Color.h"
#include "../../image/GridAtlas.h"

namespace Core {

    // forward declarations
    class Engine;
    class Texture;

    class ParticleStandardMaterial: public Material {
        friend class Engine;

    public:
        virtual Bool build() override;
        virtual void sendCustomUniformsToShader() override;
        virtual Int32 getShaderLocation(StandardAttribute attribute, UInt32 offset = 0);
        virtual Int32 getShaderLocation(StandardUniform uniform, UInt32 offset = 0);
        Int32 getWorldPositionLocation();
        Int32 getSizeLocation();
        Int32 getRotationLocation();
        Int32 getSequenceNumberLocation();
        virtual void copyTo(WeakPointer<Material> target) override;
        virtual WeakPointer<Material> clone() override;
        void bindShaderVarLocations();

        const GridAtlas& getAtlas() const;
        void setAtlas(const GridAtlas& atlas);

    private:

        ParticleStandardMaterial();

        GridAtlas atlas;
        Int32 atlasTextureLocation;
        Int32 atlasHorizontalSectionsLocation;
        Int32 atlasVerticalSectionsLocation;
        Int32 worldPositionLocation;
        Int32 sizeLocation;
        Int32 rotationLocation;
        Int32 sequenceNumberLocation;
        Int32 projectionMatrixLocation;
        Int32 viewMatrixLocation;

    };
}
