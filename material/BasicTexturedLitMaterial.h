#pragma once

#include "../util/WeakPointer.h"
#include "Material.h"

namespace Core {

    // forward declarations
    class Engine;
    class Texture;

    class BasicTexturedLitMaterial : public Material {
        friend class Engine;

    public:
        virtual Bool build() override;
        virtual Int32 getShaderLocation(StandardAttribute attribute) override;
        virtual Int32 getShaderLocation(StandardUniform uniform) override;
        virtual void sendCustomUniformsToShader() override;
        virtual WeakPointer<Material> clone() override;
        void setTexture(WeakPointer<Texture> texture);

    protected:
        BasicTexturedLitMaterial(WeakPointer<Graphics> graphics);
        void bindShaderVarLocations();

        WeakPointer<Texture> texture;
        Int32 positionLocation;
        Int32 normalLocation;
        Int32 colorLocation;
        Int32 textureLocation;
        Int32 uvLocation;
        Int32 projectionMatrixLocation;
        Int32 viewMatrixLocation;
        Int32 modelMatrixLocation;
        Int32 modelInverseTransposeMatrixLocation;

        Int32 lightPositionLocation;
        Int32 lightRangeLocation;
        Int32 lightTypeLocation;
        Int32 lightIntensityLocation;
        Int32 lightColorLocation;
        Int32 lightEnabledLocation;
        Int32 lightMatrixLocation;
        Int32 lightShadowMapLocation;
        Int32 lightShadowCubeMapLocation;
        Int32 lightShadowBiasLocation;
    };
}
