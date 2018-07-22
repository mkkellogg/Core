#include "DirectionalLight.h"
#include "../Engine.h"
#include "../render/RenderTarget2D.h"
#include "../render/RenderTargetCube.h"
#include "../common/Exception.h"
#include "../render/Camera.h"
#include "../geometry/Vector3.h"
#include "../math/Math.h"

namespace Core {

    DirectionalLight::DirectionalLight(WeakPointer<Object3D> owner, UInt32 cascadeCount, Bool shadowsEnabled, UInt32 shadowMapSize, Real shadowBias): 
        ShadowLight(owner, LightType::Directional, shadowsEnabled, shadowMapSize, shadowBias) {
        this->cascadeCount = Math::min(Constants::MaxDirectionalCascades, cascadeCount);
        for (UInt32 i = 0; i < this->cascadeCount; i++) {
            this->projections.push_back(DirectionalLight::OrthoProjection());
            this->projectionMatrices.push_back(Matrix4x4());
            this->cascadeBoundaries.push_back(0.0f);
        }
        // [cascadeBoundaries] gets 1 extra
        this->cascadeBoundaries.push_back(0.0f);
    }

    DirectionalLight::~DirectionalLight() {
    }

    void DirectionalLight::init() {
        if (this->shadowsEnabled) {
            this->buildShadowMaps();
        }
    }

    void DirectionalLight::setShadowsEnabled(Bool enabled) {
        ShadowLight::setShadowsEnabled(enabled);
        if (enabled && !this->shadowMaps.size() == 0) {
            this->buildShadowMaps();
        }
    }

    UInt32 DirectionalLight::getCascadeCount() {
        return this->cascadeCount;
    }

    std::vector<DirectionalLight::OrthoProjection>& DirectionalLight::buildProjections(WeakPointer<Camera> targetCamera) {
        if (!this->shadowsEnabled) {
            throw Exception("DirectionalLight::buildProjections() -> Cannot build shadow map projections for non-shadow-casting light");
        }

        Real near = targetCamera->getNear();
        Real far = targetCamera->getFar();

        Real frustumLength = far - near;
        Real currentBoundary = near;
        Real currentFraction = 1.0f / (Real)this->cascadeCount;
        UInt32 boundaryIndex = 0;
        while (currentBoundary < far) {
            this->cascadeBoundaries[boundaryIndex] = currentBoundary;
            currentBoundary += (currentFraction * frustumLength);
            currentFraction += 0.1f;
            boundaryIndex++;
        }
        this->cascadeBoundaries[boundaryIndex] = far;

        WeakPointer<Object3D> targetCameraOwner = targetCamera->getOwner();
        if (!targetCameraOwner.isValid()) {
            throw Exception("DirectionalLight::buildProjections() -> Target camera not attached to scene object.");
        }

        WeakPointer<Object3D> lightOwner = this->getOwner();
        if (!lightOwner.isValid()) {
            throw Exception("DirectionalLight::buildProjections() -> Light not attached to scene object.");
        }

        targetCameraOwner->getTransform().updateWorldMatrix();
        Matrix4x4 targetCameraTransform = targetCameraOwner->getTransform().getWorldMatrix();

        lightOwner->getTransform().updateWorldMatrix();
        Matrix4x4 lightTransform = lightOwner->getTransform().getWorldMatrix();
        Matrix4x4 lightTransformInverse = lightTransform;
        lightTransformInverse.invert();

        Bool isOrtho = targetCamera->isOrtho();
        for (UInt32 i = 1; i <= boundaryIndex; i++) {
            Real aspectRatio = isOrtho ? 1.0 : targetCamera->getAspectRatio();

            // TODO: support ortho rendering cmaeras!!!
            if (isOrtho) {

            }
            else {
                Real fov = targetCamera->getFOV();
                Real tanHalfHFOV = Math::tan(fov / 2.0f);
                Real tanHalfVFOV = Math::tan((fov * aspectRatio) / 2.0f);

                Real xn = this->cascadeBoundaries[i - 1] * tanHalfHFOV;
                Real xf = this->cascadeBoundaries[i] * tanHalfHFOV;
                Real yn = this->cascadeBoundaries[i - 1] * tanHalfVFOV;
                Real yf = this->cascadeBoundaries[i] * tanHalfVFOV;

                const UInt32 NumFrustumCorners = 8;
                Point3r frustumCorners[NumFrustumCorners] = {
                    // near face
                    Point3r(xn, yn, this->cascadeBoundaries[i - 1]),
                    Point3r(-xn, yn, this->cascadeBoundaries[i - 1]),
                    Point3r(xn, -yn, this->cascadeBoundaries[i - 1]),
                    Point3r(-xn, -yn, this->cascadeBoundaries[i - 1]),

                    // far face
                    Point3r(xf, yf, this->cascadeBoundaries[i]),
                    Point3r(-xf, yf, this->cascadeBoundaries[i]),
                    Point3r(xf, -yf, this->cascadeBoundaries[i]),
                    Point3r(-xf, -yf, this->cascadeBoundaries[i]) 
                };

                Point3r frustumCornersL[NumFrustumCorners];

                float minX = 0.0f;
                float maxX = 0.0f;
                float minY = 0.0f;
                float maxY = 0.0f;
                float minZ = 0.0f;
                float maxZ = 0.0f;

                for (UInt32 j = 0 ; j < NumFrustumCorners ; j++) {

                    // Transform the frustum coordinate from view to world space
                    Point3r vW = frustumCorners[j];
                    targetCameraTransform.transform(vW);

                    // Transform the frustum coordinate from world to light space
                    lightTransformInverse.transform(vW);
                    Point3r corner =  vW;

                    minX = j == 0 ? corner.x : Math::min(minX, corner.x);
                    maxX = j == 0 ? corner.x : Math::max(maxX, corner.x);
                    minY = j == 0 ? corner.y : Math::min(minY, corner.y);
                    maxY = j == 0 ? corner.y : Math::max(maxY, corner.y);
                    minZ = j == 0 ? corner.z : Math::min(minZ, corner.z);
                    maxZ = j == 0 ? corner.z : Math::max(maxZ, corner.z);

                }

                //std::cerr << " [" << minX << ", " << maxX << "]   [" << minY << ", " << maxY << "]  [" << minZ << ", " << maxZ << "] " << std::endl;

                OrthoProjection& oProj = this->projections[i - 1];

                oProj.right = maxX;
                oProj.left = minX;
                oProj.bottom = minY;
                oProj.top = maxY;
                oProj.far = maxZ;
                oProj.near = minZ;

                Camera::buildOrthographicProjectionMatrix(oProj.top, oProj.bottom, oProj.left, oProj.right, oProj.near, oProj.far, this->projectionMatrices[i - 1]);                
            }
        }   

        return this->projections;
    }

    Matrix4x4& DirectionalLight::getProjectionMatrix(UInt32 cascadeIndex) {
        if (cascadeIndex >= this->cascadeCount) {
            throw OutOfRangeException("DirectionalLight::getProjectionMatrix() -> 'cascadeIndex' is out of range.");
        }
        return this->projectionMatrices[cascadeIndex];
    }

    Real DirectionalLight::getCascadeBoundary(UInt32 boundaryIndex) {
        if (boundaryIndex >= this->cascadeBoundaries.size()) {
            throw OutOfRangeException("DirectionalLight::getCascadeBoundary() -> 'boundaryIndex' is out of range.");
        }
        return this->cascadeBoundaries[boundaryIndex];
    }

    WeakPointer<RenderTarget> DirectionalLight::getShadowMap(UInt32 cascadeIndex) {
        if (cascadeIndex >= this-> cascadeCount) {
            throw OutOfRangeException("DirectionalLight::getShadowMap() -> 'cascadeIndex' is out of range.");
        }
        return this->shadowMaps[cascadeIndex];
    }

    void DirectionalLight::buildShadowMaps() {
        TextureAttributes colorTextureAttributes;
        colorTextureAttributes.Format = TextureFormat::R32F;
        colorTextureAttributes.FilterMode = TextureFilter::Linear;
        Vector2u renderTargetSize(this->shadowMapSize, this->shadowMapSize);
        for (UInt32 i = 0; i < this->cascadeCount; i++) {
            auto graphics = Engine::instance()->getGraphicsSystem();
            PersistentWeakPointer<RenderTarget2D> map = graphics->createRenderTarget2D(true, true, false, colorTextureAttributes, renderTargetSize);
            this->shadowMaps.push_back(map);
        }
    }

}