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

    class SSAOMaterial : public BaseMaterial {
        friend class Engine;

    public:
        virtual ~SSAOMaterial();
        virtual Bool build() override;
        virtual Int32 getShaderLocation(StandardAttribute attribute, UInt32 offset = 0) override;
        virtual void sendCustomUniformsToShader() override;
        virtual WeakPointer<Material> clone() override;
        virtual void copyTo(WeakPointer<Material> targetMaterial) override;
        virtual void bindShaderVarLocations() override;

        void setViewPositions(WeakPointer<Texture> positions);
        void setViewNormals(WeakPointer<Texture> normals);
        void setNoise(WeakPointer<Texture> noise);
        void setSamples(const std::vector<Vector3r>& samples);
        void setProjection(const Matrix4x4& projection);
        void setRadius(Real radius);
        void setBias(Real bias);
        void setScreenWidth(Real screenWidth);
        void setScreenHeight(Real screenHeight);

    private:
        SSAOMaterial();

        Int32 viewPositionsLocation;
        Int32 viewNormalsLocation;
        Int32 noiseLocation;
        Int32 samplesLocation[Constants::SSAOSamples];
        Int32 projectionLocation;
        Int32 radiusLocation;
        Int32 biasLocation;
        Int32 screenWidthLocation;
        Int32 screenHeightLocation;
        Int32 albedoUVLocation;

        PersistentWeakPointer<Texture> viewPositions;
        PersistentWeakPointer<Texture> viewNormals;
        PersistentWeakPointer<Texture> noise;
        std::vector<Vector3r> samples;
        Matrix4x4 projection;
        Real radius;
        Real bias;
        Real screenWidth;
        Real screenHeight;
    };
}
