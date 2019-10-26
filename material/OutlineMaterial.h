#pragma once

#include "../util/WeakPointer.h"
#include "../common/Constants.h"
#include "Material.h"
#include "../color/Color.h"

namespace Core {

    // forward declarations
    class Engine;

    class OutlineMaterial: public Material {
        friend class Engine;

    public:
        virtual Bool build() override;
        virtual Int32 getShaderLocation(StandardAttribute attribute, UInt32 offset = 0) override;
        virtual Int32 getShaderLocation(StandardUniform uniform, UInt32 offset = 0) override;
        virtual void sendCustomUniformsToShader() override;
        virtual WeakPointer<Material> clone() override;

        void setColor(Color color);
        void setEdgeWidth(Real width);
        void setPctExtend(Real extend);
        void setAbsExtend(Real extend);

    private:
        OutlineMaterial(WeakPointer<Graphics> graphics);
        void bindShaderVarLocations();

        Color color;
        Real edgeWidth;
        Real pctExtend;
        Real absExtend;

        Int32 positionLocation;
        Int32 projectionMatrixLocation;
        Int32 viewMatrixLocation;
        Int32 modelMatrixLocation;
        Int32 colorLocation;
        Int32 edgeWidthLocation;
        Int32 pctExtendLocation;
        Int32 absExtendLocation;

        Int32 skinningEnabledLocation;
        Int32 bonesLocation[Constants::MaxBones];
        Int32 boneIndexLocation;
        Int32 boneWeightLocation;
    };
}
