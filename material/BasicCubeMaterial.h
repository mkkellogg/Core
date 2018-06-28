#pragma once

#include "../util/WeakPointer.h"
#include "Material.h"

namespace Core {

    // forward declarations
    class Engine;
    class CubeTexture;

    class BasicCubeMaterial : public Material {
        friend class Engine;

    public:
        virtual Bool build() override;
        virtual Int32 getShaderLocation(StandardAttribute attribute) override;
        virtual Int32 getShaderLocation(StandardUniform uniform) override;
        virtual void sendCustomUniformsToShader() override;
        virtual WeakPointer<Material> clone() override;
        void setTexture(WeakPointer<CubeTexture> texture);

    private:
        BasicCubeMaterial(WeakPointer<Graphics> graphics);
        Int32 positionLocation;
        Int32 colorLocation;
        Int32 textureLocation;
        Int32 projectionMatrixLocation;
        Int32 viewMatrixLocation;
        PersistentWeakPointer<CubeTexture> texture;
    };
}
