#pragma once

#include <vector>

#include "BaseMaterial.h"
#include "../util/WeakPointer.h"
#include "../math/Matrix4x4.h"
#include "../geometry/Vector3.h"

namespace Core {

    // forward declarations
    class Engine;
    class Texture;

    class BufferOutlineMaterial : public BaseMaterial {
        friend class Engine;

    public:
        virtual ~BufferOutlineMaterial();
        virtual Bool build() override;
        virtual Int32 getShaderLocation(StandardUniform uniform, UInt32 offset = 0) override;
        virtual void sendCustomUniformsToShader() override;
        virtual WeakPointer<Material> clone() override;
        virtual void copyTo(WeakPointer<Material> targetMaterial) override;
        virtual void bindShaderVarLocations() override;

        void setOutlineColor(Color color);
        void setOutlineSize(UInt32 outlineSize);

    private:
        BufferOutlineMaterial();

        Int32 silhouetteLocation;
        Int32 outlineColorLocation;
        Int32 outlineSizeLocation;

        Color outlineColor;
        UInt32 outlineSize;
    };
}
