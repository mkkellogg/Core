#pragma once

#include "../util/WeakPointer.h"
#include "Material.h"

namespace Core {

    // forward declarations
    class Engine;
    class Texture2D;

    class EquirectangularMaterial : public Material {
        friend class Engine;

    public:
        virtual ~EquirectangularMaterial();
        virtual Bool build() override;
        virtual Int32 getShaderLocation(StandardAttribute attribute, UInt32 offset = 0) override;
        virtual Int32 getShaderLocation(StandardUniform uniform, UInt32 offset = 0) override;
        virtual void sendCustomUniformsToShader() override;
        virtual WeakPointer<Material> clone() override;
        void setTexture(WeakPointer<Texture2D> texture);
        virtual UInt32 textureCount() override;

    private:
        EquirectangularMaterial(WeakPointer<Graphics> graphics);
        void bindShaderVarLocations();
        
        Int32 positionLocation;
        Int32 colorLocation;
        Int32 projectionMatrixLocation;
        Int32 viewMatrixLocation;
        Int32 modelMatrixLocation;
        Int32 textureLocation;
        PersistentWeakPointer<Texture2D> texture;
    };
}
