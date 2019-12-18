#pragma once

#include "../util/WeakPointer.h"
#include "BaseMaterial.h"

namespace Core {

    // forward declarations
    class Engine;
    class Texture2D;

    class EquirectangularMaterial : public BaseMaterial {
        friend class Engine;

    public:
        virtual ~EquirectangularMaterial();
        virtual Bool build() override;
        virtual void sendCustomUniformsToShader() override;
        virtual void copyTo( WeakPointer<Material> target) override;
        virtual WeakPointer<Material> clone() override;
        virtual UInt32 textureCount() override;
        virtual void bindShaderVarLocations() override;

        void setTexture(WeakPointer<Texture2D> texture);

    private:
        EquirectangularMaterial(WeakPointer<Graphics> graphics);
        
        Int32 textureLocation;
        PersistentWeakPointer<Texture2D> texture;
    };
}
