#pragma once

#include "../util/PersistentWeakPointer.h"
#include "../common/types.h"
#include "../geometry/Vector3.h"
#include "../math/Matrix4x4.h"
#include "../scene/Object3DComponent.h"
#include "../render/RenderBuffer.h"
#include "../render/ToneMapType.h"
#include "../base/BitMask.h"
#include "../geometry/Ray.h"
#include "../scene/Skybox.h"
#include "../image/CubeTexture.h"

namespace Core {

    // forward declarations
    class Engine;
    class RenderTarget;

    class Camera : public Object3DComponent {
        friend class Engine;
        friend class Renderer;

    public:
        static const UInt32 DEFAULT_FOV;
        static const UInt32 DEFAULT_WIDTH;
        static const UInt32 DEFAULT_HEIGHT;
        static const Real DEFAULT_ASPECT_RATIO;
        static const Real DEFAULT_NEARP;
        static const Real DEFAULT_FARP;

        void setAspectRatio(Real ratio);       
        void setAspectRatioFromDimensions(UInt32 width, UInt32 height);
        void setDimensions(Real top, Real  bottom, Real left, Real right);
        Vector4r getDimensions() const;
        void setNear(Real near);
        void setFar(Real far);
        void setNearAndFar(Real near, Real far);
        Real getAspectRatio() const;
        Real getFOV() const;
        Real getNear();
        Real getFar();
        const Matrix4x4& getProjectionMatrix() const;
        const Matrix4x4& getWorlInverseTransposeMatrix() const;
        void lookAt(const Point3r& target);
        void project(Vector3Base<Real>& vec) const;
        void unProject(Vector3Base<Real>& vec) const;
        void setRenderTarget(WeakPointer<RenderTarget> renderTarget);
        WeakPointer<RenderTarget> getRenderTarget();
        WeakPointer<RenderTarget2D> getHDRRenderTarget();
        void setOrtho(Bool ortho);
        Bool isOrtho() const;
        void setAutoClearRenderBuffer(RenderBufferType type, Bool clear);
        Bool getAutoClearRenderBuffer(RenderBufferType type) const;
        IntMask getAutoClearRenderBuffers() const;
        Ray getRay(const Vector4u& viewport, Core::Int32 x, Core::Int32 y);
        void copyFrom(WeakPointer<Camera> other);
        Skybox& getSkybox();
        void setSkybox(Skybox& skybox);
        void setSkyboxEnabled(Bool enabled);
        Bool isSkyboxEnabled();
        void setHDREnabled(Bool enabled);
        Bool isHDREnabled();
        void setHDRToneMapTypeReinhard();
        void setHDRToneMapTypeExposure(Real exposure);
        ToneMapType getHDRToneMapType();
        Real getHDRExposure();
        void setHDRGamma(Real gamma);
        Real getHDRGamma();

        static void buildPerspectiveProjectionMatrix(Real fov, Real aspectRatio, Real near, Real far, Matrix4x4& out);
        static void buildOrthographicProjectionMatrix(Real top, Real bottom, Real left, Real right, Real near, Real far, Matrix4x4& matrix);

        static Vector2r ndcToViewport(const Point3r& ndcCoords, const Vector4u& viewport);
        static Vector2r ndcToViewport(const Vector2r& ndcCoords, const Vector4u& viewport);
        static Vector2r viewportToNDC(const Vector2r& viewportCoords, const Vector4u& viewport);

    private:
        Camera(WeakPointer<Object3D> owner);
        void updateProjection();
        void buildHDRRenderTarget(const Vector2u& size);

        static Camera* createPerspectiveCamera(WeakPointer<Object3D> owner, Real fov, Real aspectRatio, Real near, Real far);
        static Camera* createOrthographicCamera(WeakPointer<Object3D> owner, Real top, Real bottom, Real left, Real right, Real near, Real far);

        Bool ortho;

        Real fov;
        Real aspectRatio;

        Real near;
        Real far;

        Vector4r dimensions;

        Matrix4x4 projectionMatrix;
        PersistentWeakPointer<RenderTarget> renderTarget;
        PersistentWeakPointer<RenderTarget2D> hdrRenderTarget;
        IntMask clearRenderBuffers;

        Bool skyboxEnabled;
        Skybox skybox;

        Bool hdrEnabled;
        ToneMapType hdrToneMapType;
        Real hdrExposure;
        Real hdrGamma;
    };
}
