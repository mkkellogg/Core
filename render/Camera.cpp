#include "string.h"

#include "Camera.h"
#include "../common/types.h"
#include "../math/Math.h"
#include "../math/Matrix4x4.h"
#include "../math/Quaternion.h"
#include "../util/WeakPointer.h"
#include "../scene/Object3D.h"
#include "../render/RenderTarget2D.h"

namespace Core {

    const UInt32 Camera::DEFAULT_FOV = 70;
    const UInt32 Camera::DEFAULT_WIDTH = 1200;
    const UInt32 Camera::DEFAULT_HEIGHT = 800;
    const Real Camera::DEFAULT_ASPECT_RATIO = (Real)Camera::DEFAULT_WIDTH / (Real)Camera::DEFAULT_HEIGHT;
    const Real Camera::DEFAULT_NEARP = 0.1;
    const Real Camera::DEFAULT_FARP = 100.0;

    Camera::Camera(WeakPointer<Object3D> owner): Object3DComponent(owner), skyboxEnabled(false), hdrEnabled(false) {
        this->setAutoClearRenderBuffer(RenderBufferType::Color, true);
        this->setAutoClearRenderBuffer(RenderBufferType::Depth, true);
        this->setAutoClearRenderBuffer(RenderBufferType::Stencil, true);
        this->hdrToneMapType = ToneMapType::Reinhard;
        this->hdrExposure = 1.0f;
        this->hdrGamma = 2.2f;
        this->ssaoEnabled = false;
        this->ssaoRadius = 1.5f;
        this->ssaoBias = 0.05f;
    }

    Camera::~Camera() {
        if (this->hdrRenderTarget.isValid()) Graphics::safeReleaseObject(hdrRenderTarget);
    }

    void Camera::setAspectRatio(Real ratio) {
        this->aspectRatio = ratio;
        this->updateProjection();
    }

    void Camera::setAspectRatioFromDimensions(UInt32 width, UInt32 height) {
        this->aspectRatio = (Real)width / (Real)height;
        this->updateProjection();
    }

    void Camera::setDimensions(Real top, Real  bottom, Real left, Real right) {
        this->dimensions.set(left, top, right, bottom);
        this->updateProjection();
    }

    Vector4r Camera::getDimensions() const {
        return this->dimensions;
    }

    void Camera::setNear(Real near) {
        this->setNearAndFar(near, this->far);
    }

    void Camera::setFar(Real far) {
        this->setNearAndFar(this->near, far);
    }

    void Camera::setNearAndFar(Real near, Real far) {
        this->near = near;
        this->far = far;
        this->updateProjection();
    }

    Real Camera::getAspectRatio() const {
        return this->aspectRatio;
    }

    Real Camera::getFOV() const {
        return this->fov;
    }

    Real Camera::getNear() {
        return this->near;
    }

    Real Camera::getFar() {
        return this->far;
    }

    const Matrix4x4& Camera::getProjectionMatrix() const {
        return this->projectionMatrix;
    }

    void Camera::lookAt(const Point3r& target) {
        WeakPointer<Object3D> owner(this->getOwner());
        owner->getTransform().lookAt(target);
    }

    void Camera::project(Vector3Base<Real>& vec) const {
        Core::Matrix4x4 projection = this->projectionMatrix;
        Real w = vec.getW();
        projection.transform(vec);
    }

    void Camera::unProject(Vector3Base<Real>& vec) const {
        Core::Matrix4x4 projection = this->projectionMatrix;
        projection.invert();
        projection.transform(vec);
    }

    Point3r Camera::unProject(Vector2u& vec, Real ndcZ) const {
        return this->unProject(vec.x, vec.y, ndcZ);
    }

    Point3r Camera::unProject(UInt32 screenX, UInt32 screenY, Real ndcZ) const {
        WeakPointer<Graphics> graphics = Engine::instance()->getGraphicsSystem();
        WeakPointer<RenderTarget> cameraRenderTarget = this->getRenderTarget();
        if (!cameraRenderTarget.isValid()) {
            cameraRenderTarget = graphics->getDefaultRenderTarget();
        }
        Vector4u viewport = cameraRenderTarget->getViewport();
        Point3r ndcPos = screenToNDC(screenX, screenY, ndcZ, viewport);
        this->unProject(ndcPos);
        return ndcPos;
    }

    void Camera::setRenderTarget(WeakPointer<RenderTarget> renderTarget) {
        this->renderTarget = renderTarget;
    }

    WeakPointer<RenderTarget> Camera::getRenderTarget() const {
        return this->renderTarget;
    }
    
    WeakPointer<RenderTarget2D> Camera::getHDRRenderTarget() const {
        return this->hdrRenderTarget;
    }

    void Camera::setOrtho(Bool ortho) {
        this->ortho = ortho;
    }

    Bool Camera::isOrtho() const {
        return this->ortho;
    }

    void Camera::setAutoClearRenderBuffer(RenderBufferType type, Bool clear) {
        if (clear) {
            IntMaskUtil::setBitForMask(&this->clearRenderBuffers, (UInt32)type);
        }
        else {
            IntMaskUtil::clearBitForMask(&this->clearRenderBuffers, (UInt32)type);
        }
    }

    Bool Camera::getAutoClearRenderBuffer(RenderBufferType type) const {
        return IntMaskUtil::isBitSetForMask(this->clearRenderBuffers, (UInt32)type);
    }

    IntMask Camera::getAutoClearRenderBuffers() const {
        return this->clearRenderBuffers;
    }

    Ray Camera::getRay(Core::Int32 x, Core::Int32 y) {
        Core::Point3r viewPos = this->unProject(x, y, -1.0f);
        Core::Transform& camTransform = this->getOwner()->getTransform();
        camTransform.updateWorldMatrix();

        Core::Point3r worldPos = viewPos;
        camTransform.getWorldMatrix().transform(worldPos);
        Core::Point3r origin;
        camTransform.getWorldMatrix().transform(origin);
        Core::Vector3r rayDir = worldPos - origin;
        rayDir.normalize();
        Core::Ray ray(origin, rayDir);
        return ray;
    }

    void Camera::copyFrom(WeakPointer<Camera> other) {
        this->ortho = other->ortho;
        this->fov = other->fov;
        this->aspectRatio = other->aspectRatio;
        this->near = other->near;
        this->far = other->far;
        this->dimensions = other->dimensions;
        this->skybox = other->skybox;
        this->skyboxEnabled = other->skyboxEnabled;
        this->hdrEnabled = other->hdrEnabled;
        this->projectionMatrix.copy(other->projectionMatrix);

        // TODO: Do we need a deep copy here?
        this->renderTarget = other->renderTarget;
        this->clearRenderBuffers = other->clearRenderBuffers;
    }

    Skybox& Camera::getSkybox() {
        return this->skybox;
    }

    void Camera::setSkybox(Skybox& skybox) {
        this->skybox = skybox;
    }

    void Camera::setSkyboxEnabled(Bool enabled) {
        this->skyboxEnabled = enabled;
    }

    Bool Camera::isSkyboxEnabled() const {
        return this->skyboxEnabled;
    }

    void Camera::setHDREnabled(Bool enabled) {
        this->hdrEnabled = enabled;
    }

    Bool Camera::isHDREnabled() const {
        return this->hdrEnabled;
    }
    
    void Camera::setHDRToneMapTypeReinhard() {
        this->hdrToneMapType = ToneMapType::Reinhard;
    }

    void Camera::setHDRToneMapTypeExposure(Real exposure) {
        this->hdrToneMapType = ToneMapType::Exposure;
        this->hdrExposure = exposure;
    }

    ToneMapType Camera::getHDRToneMapType() const {
        return this->hdrToneMapType;
    }

    Real Camera::getHDRExposure() const {
        return this->hdrExposure;
    }

    void Camera::setHDRGamma(Real gamma) {
        this->hdrGamma = gamma;
    }

    Real Camera::getHDRGamma() const {
        return this->hdrGamma;
    }

    void Camera::setSSAOEnabled(Bool enabled) {
        this->ssaoEnabled = enabled;
    }

    Bool Camera::isSSAOEnabled() const {
        return this->ssaoEnabled;
    }

    void Camera::setSSAORadius(Real radius) {
        this->ssaoRadius = radius;
    }

    Real Camera::getSSAORadius() {
        return this->ssaoRadius;
    }

    void Camera::setSSAOBias(Real bias) {
        this->ssaoBias = bias;
    }

    Real Camera::getSSAOBias() {
        return this->ssaoBias;
    }

    void Camera::buildPerspectiveProjectionMatrix(Real fov, Real ratio, Real nearP, Real farP, Matrix4x4& out) {
        // convert fov to radians
        Real f = 1.0f / Math::tan(fov * .5f);

        Real data[16];
        memset(data, 0, 16 * sizeof(Real));

        data[0] = f / ratio;
        data[1 * 4 + 1] = f;
        data[2 * 4 + 2] = (farP + nearP) / (nearP - farP);
        data[3 * 4 + 2] = (2.0f * farP * nearP) / (nearP - farP);
        data[2 * 4 + 3] = -1.0f;
        data[3 * 4 + 3] = 0.0f;
        out.copy(data);
    }

    void Camera::buildOrthographicProjectionMatrix(Real top, Real bottom, Real left, Real right, Real near, Real far, Matrix4x4& matrix) {
        matrix.setIdentity();

        Real data[16];
        memset(data, 0, 16 * sizeof(Real));

        Real r_width = 1.0f / (right - left);
        Real r_height = 1.0f / (top - bottom);
        Real r_depth = 1.0f / (far - near);
        Real x = 2.0f * (r_width);
        Real y = 2.0f * (r_height);
        Real z = -2.0f * (r_depth);
        Real tx = -(right + left) * r_width;
        Real ty = -(top + bottom) * r_height;
        Real tz = -(far + near) * r_depth;
        data[0] = x;
        data[5] = y;
        data[10] = z;
        data[12] = tx;
        data[13] = ty;
        data[14] = tz;
        data[15] = 1.0f;
        data[1] = 0.0f;
        data[2] = 0.0f;
        data[3] = 0.0f;
        data[4] = 0.0f;
        data[6] = 0.0f;
        data[7] = 0.0f;
        data[8] = 0.0f;
        data[9] = 0.0f;
        data[11] = 0.0f;

        matrix.copy(data);
    }

    Vector2r Camera::ndcToScreen(const Point3r& ndcCoords, const Vector4u& viewport) {
        return ndcToScreen(Vector2r(ndcCoords.x, ndcCoords.y), viewport);
    }

    Vector2r Camera::ndcToScreen(const Vector2r& ndcCoords, const Vector4u& viewport) {
        Real vpX =  ((ndcCoords.x + 1.0f) / 2.0f) * (Real)viewport.z;
        Real vpY =  ((ndcCoords.y + 1.0f) / 2.0f) * (Real)viewport.w;
        return Vector2r(vpX, (Real)viewport.w - vpY);
    }

    Point3r Camera::screenToNDC(const Vector2u& screenCoords, Real ndcZ, const Vector4u& viewport) {
        return screenToNDC(screenCoords.x, screenCoords.y, ndcZ, viewport);
    }

    Point3r Camera::screenToNDC(UInt32 screenX, UInt32 screenY, Real ndcZ, const Vector4u& viewport) {
        screenY = viewport.w - screenY;
        Real ndcX = ((Real)screenX / (Real)viewport.z) * 2.0f - 1.0f;
        Real ndcY = ((Real)screenY / (Real)viewport.w) * 2.0f - 1.0f;
        return Point3r(ndcX, ndcY, ndcZ);
    }

    void Camera::updateProjection() {
        if (this->ortho) {
            Camera::buildOrthographicProjectionMatrix(dimensions.y, dimensions.w, dimensions.x, dimensions.z, this->near, this->far, this->projectionMatrix);
        }
        else {
            Camera::buildPerspectiveProjectionMatrix(this->fov, this->aspectRatio, this->near, this->far, this->projectionMatrix);
        }
    }

    void Camera::buildHDRRenderTarget(const Vector2u& size) {
        if (this->hdrRenderTarget) {
            Graphics::safeReleaseObject(this->hdrRenderTarget);
        }
        TextureAttributes hdrColorAttributes;
        hdrColorAttributes.Format = TextureFormat::RGBA16F;
        hdrColorAttributes.FilterMode = TextureFilter::Point;
        hdrColorAttributes.MipLevels = 0;
        hdrColorAttributes.WrapMode = TextureWrap::Clamp;
        TextureAttributes hdrDepthAttributes;
        hdrDepthAttributes.IsDepthTexture = true;
        this->hdrRenderTarget = Engine::instance()->getGraphicsSystem()->createRenderTarget2D(true, true, false, hdrColorAttributes, hdrDepthAttributes, size);
    }

    Camera* Camera::createPerspectiveCamera(WeakPointer<Object3D> owner, Real fov, Real aspectRatio, Real near, Real far) {
        Camera * cam = new(std::nothrow) Camera(owner);
        if (cam == nullptr) {
            throw AllocationException("Camera::createPerspectiveCamera -> Could not allocation new Camera object.");
        }
        cam->fov = fov;
        cam->aspectRatio = aspectRatio;
        cam->near = near;
        cam->far = far;
        cam->ortho = false;
        Camera::buildPerspectiveProjectionMatrix(fov, aspectRatio, near, far, cam->projectionMatrix);
        return cam;
    }

    Camera* Camera::createOrthographicCamera(WeakPointer<Object3D> owner, Real top, Real bottom, Real left, Real right, Real near, Real far) {
        Camera * cam = new(std::nothrow) Camera(owner);
        if (cam == nullptr) {
            throw AllocationException("Camera::createOrthographicCamera -> Could not allocation new Camera object.");
        }
        cam->dimensions.set(left, top, right, bottom);
        cam->near = near;
        cam->far = far;
        cam->ortho = true;
        Camera::buildOrthographicProjectionMatrix(top, bottom, left, right, near, far, cam->projectionMatrix);
        return cam;
    }
}
