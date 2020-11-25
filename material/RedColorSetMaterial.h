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

    class RedColorSetMaterial : public BaseMaterial {
        friend class Engine;

    public:
        virtual ~RedColorSetMaterial();
        virtual Bool build() override;
        virtual Int32 getShaderLocation(StandardUniform uniform, UInt32 offset = 0) override;
        virtual void sendCustomUniformsToShader() override;
        virtual WeakPointer<Material> clone() override;
        virtual void copyTo(WeakPointer<Material> targetMaterial) override;
        virtual void bindShaderVarLocations() override;

        void setOutputColor(Color outputColor);

    private:
        RedColorSetMaterial();

        Int32 inputLocation;
        Int32 outputColorLocation;
        Color outputColor;
    };
}
