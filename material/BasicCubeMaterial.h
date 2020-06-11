#pragma once

#include "../util/WeakPointer.h"
#include "BaseMaterial.h"

namespace Core {

    // forward declarations
    class Engine;
    class CubeTexture;
    class Texture2D;

    class BasicCubeMaterial : public BaseMaterial {
        friend class Engine;

    public:
        virtual ~BasicCubeMaterial();
        virtual Bool build() override;
        virtual void sendCustomUniformsToShader() override;
        virtual void copyTo(WeakPointer<Material> target) override;
        virtual WeakPointer<Material> clone() override;
        virtual UInt32 textureCount() override;
        virtual void bindShaderVarLocations() override;

        void setCubeTexture(WeakPointer<CubeTexture> texture);
        void setRectTexture(WeakPointer<Texture2D> texture);

    private:
        BasicCubeMaterial(WeakPointer<Graphics> graphics);

        Int32 cubeTextureLocation;
        Int32 rectTextureLocation;
        PersistentWeakPointer<CubeTexture> cubeTexture;
        PersistentWeakPointer<Texture2D> rectTexture;
    };
}
