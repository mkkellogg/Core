#pragma once

#include "../util/WeakPointer.h"
#include "../common/Constants.h"
#include "Material.h"
#include "../color/Color.h"

namespace Core {

    // forward declarations
    class Engine;

    class BaseMaterial: public Material {
        friend class Engine;

    public:
        virtual ~BaseMaterial();
        virtual Int32 getShaderLocation(StandardAttribute attribute, UInt32 offset = 0) override;
        virtual Int32 getShaderLocation(StandardUniform uniform, UInt32 offset = 0) override;
        virtual void bindShaderVarLocations();
        virtual void copyTo(WeakPointer<Material> target) override;

    protected:
        BaseMaterial(WeakPointer<Graphics> graphics);

        Int32 positionLocation;
        Int32 normalLocation;
        Int32 averagedNormalLocation;
        Int32 faceNormalLocation;
        Int32 colorLocation;
        Int32 tangentLocation;

        Int32 projectionMatrixLocation;
        Int32 viewMatrixLocation;
        Int32 modelMatrixLocation;
        Int32 modelInverseTransposeMatrixLocation;
        Int32 cameraPositionLocation;

        Int32 skinningEnabledLocation;
        Int32 bonesLocation[Constants::MaxBones];
        Int32 boneIndexLocation;
        Int32 boneWeightLocation;
    };
}
