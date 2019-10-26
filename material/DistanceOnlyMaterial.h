#pragma once

#include "../util/WeakPointer.h"
#include "../common/Constants.h"
#include "Material.h"

namespace Core {

    // forward declarations
    class Engine;

    class DistanceOnlyMaterial : public Material {
        friend class Engine;

    public:
        virtual Bool build() override;
        virtual Int32 getShaderLocation(StandardAttribute attribute, UInt32 offset = 0) override;
        virtual Int32 getShaderLocation(StandardUniform uniform, UInt32 offset = 0) override;
        virtual void sendCustomUniformsToShader() override;
        virtual WeakPointer<Material> clone() override;

    private:
        DistanceOnlyMaterial(WeakPointer<Graphics> graphics);
        void bindShaderVarLocations();

        Int32 positionLocation;
        Int32 projectionMatrixLocation;
        Int32 viewMatrixLocation;
        Int32 modelMatrixLocation;

        Int32 skinningEnabledLocation;
        Int32 bonesLocation[Constants::MaxBones];
        Int32 boneIndexLocation;
        Int32 boneWeightLocation;
    };
}
