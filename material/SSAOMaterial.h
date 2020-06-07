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
        virtual void bindShaderVarLocations() override;

        void setViewPositions(WeakPointer<Texture> positions);
        void setViewNormals(WeakPointer<Texture> normals);
        void setNoise(WeakPointer<Texture> noise);
        void setSamples(const std::vector<Vector3r>& samples);
        void setProjection(const Matrix4x4& projection);

    private:
        SSAOMaterial(WeakPointer<Graphics> graphics);

        Int32 viewPositionsLocation;
        Int32 viewNormalsLocation;
        Int32 noiseLocation;
        Int32 samplesLocation[Constants::SSAOSamples];
        Int32 projectionLocation;
        Int32 albedoUVLocation;

        PersistentWeakPointer<Texture> viewPositions;
        PersistentWeakPointer<Texture> viewNormals;
        PersistentWeakPointer<Texture> noise;
        std::vector<Vector3r> samples;
        Matrix4x4 projection;
    };
}
