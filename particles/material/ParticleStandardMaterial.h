#pragma once

#include "../../util/WeakPointer.h"
#include "../../common/Constants.h"
#include "../../material/Material.h"
#include "../../color/Color.h"
#include "../../image/Atlas.h"

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
        Int32 getSequenceElementLocation();
        Int32 getColorLocation();
        virtual void copyTo(WeakPointer<Material> target) override;
        virtual WeakPointer<Material> clone() override;
        void bindShaderVarLocations();

        const Atlas& getAtlas() const;
        void setAtlas(const Atlas& atlas);

    private:

        ParticleStandardMaterial();

        Atlas atlas;
        Int32 atlasTextureLocation;
        Int32 atlasTileArayLocation[Constants::MaxAtlasTileArrays];
        Int32 worldPositionLocation;
        Int32 sizeLocation;
        Int32 rotationLocation;
        Int32 sequenceElementLocation;
        Int32 colorLocation;
        Int32 projectionMatrixLocation;
        Int32 viewMatrixLocation;

    };
}
