#include <algorithm>

#include "../common/Exception.h"
#include "GraphicsGL.h"
#include "AttributeArrayGPUStorageGL.h"
#include "CubeTextureGL.h"
#include "IndexBufferGL.h"
#include "RendererGL.h"
#include "ShaderGL.h"
#include "Texture2DGL.h"
#include "RenderTarget2DGL.h"
#include "RenderTargetCubeGL.h"

namespace Core {

    static void printGlString(const char* name, GLenum s) {
        const char* v = (const char*)glGetString(s);
        Debug::PrintMessage("GL %s: %s\n", name, v);
    }

    GraphicsGL::GraphicsGL(GLVersion version) : glVersion(version) {
        this->renderStyle = RenderStyle::Fill;
    }

    GraphicsGL::~GraphicsGL() {
    }

    void GraphicsGL::init() {
        printGlString("Version", GL_VERSION);
        printGlString("Vendor", GL_VENDOR);
        printGlString("Renderer", GL_RENDERER);
        printGlString("Extensions", GL_EXTENSIONS);

        UInt32 maxGL = 0;
        const char* versionStr = (const char*)glGetString(GL_VERSION);

        this->renderer = this->createRenderer();
        this->defaultRenderTarget = this->createDefaultRenderTarget();
        this->currentRenderTarget = this->defaultRenderTarget;
        this->shaderDirectory.init();
    }

    WeakPointer<Renderer> GraphicsGL::getRenderer() {
        return std::static_pointer_cast<Renderer>(this->renderer);
    }

    void GraphicsGL::preRender() {
        // TODO: Move these state calls to a place where they are not called every frame
       /* glFrontFace(GL_CW);
        glCullFace(GL_BACK);
        glEnable(GL_CULL_FACE);
        glEnable(GL_DEPTH_TEST);
        glDepthMask(GL_TRUE);
        glDepthFunc(GL_LEQUAL);
        glDisable(GL_BLEND);

        glLineWidth(1.5);
        glEnable(GL_LINE_SMOOTH);
        glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);*/
    }

    void GraphicsGL::postRender() {

    }

    void GraphicsGL::setViewport(UInt32 hOffset, UInt32 vOffset, UInt32 viewPortWidth, UInt32 viewPortHeight) {
        glViewport(hOffset, vOffset, viewPortWidth, viewPortHeight);
        this->viewport.set(hOffset, vOffset, viewPortWidth, viewPortHeight);
    }

    Vector4u GraphicsGL::getViewport() {
        return this->viewport;
    }

    WeakPointer<Texture2D> GraphicsGL::createTexture2D(const TextureAttributes& attributes) {
        std::shared_ptr<Texture2DGL> newTexture = std::shared_ptr<Texture2DGL>(new Texture2DGL(attributes));
        this->textures2D.push_back(newTexture);
        return std::static_pointer_cast<Texture2D>(newTexture);
    }

    WeakPointer<CubeTexture> GraphicsGL::createCubeTexture(const TextureAttributes& attributes) {
        std::shared_ptr<CubeTextureGL> newTexture = std::shared_ptr<CubeTextureGL>(new CubeTextureGL(attributes));
        this->cubeTextures.push_back(newTexture);
        return std::static_pointer_cast<CubeTexture>(newTexture);
    }

    void GraphicsGL::destroyTexture2D(WeakPointer<Texture2D> texture) {
        WeakPointer<Texture2DGL> textureGL = WeakPointer<Texture2D>::dynamicPointerCast<Texture2DGL>(texture);
        if (textureGL.isValid()) {
            std::shared_ptr<Texture2DGL> sharedPtr = textureGL.lock();
            auto end = this->textures2D.end();
            auto result = std::find(this->textures2D.begin(), end, sharedPtr);
            if (result != end) {
                this->textures2D.erase(result);
            }
        }
    }

    void GraphicsGL::destroyCubeTexture(WeakPointer<CubeTexture> texture) {
        WeakPointer<CubeTextureGL> textureGL = WeakPointer<CubeTexture>::dynamicPointerCast<CubeTextureGL>(texture);
        if (textureGL.isValid()) {
            std::shared_ptr<CubeTextureGL> sharedPtr = textureGL.lock();
            auto end = this->cubeTextures.end();
            auto result = std::find(this->cubeTextures.begin(), end, sharedPtr);
            if (result != end) {
                this->cubeTextures.erase(result);
            }
        }
    }

    WeakPointer<Shader> GraphicsGL::createShader(const std::string& vertex, const std::string& fragment) {
        std::shared_ptr<ShaderGL> shaderGL(new ShaderGL(vertex, fragment));
        shaders.push_back(shaderGL);
        std::shared_ptr<Shader> shader = std::static_pointer_cast<Shader>(shaderGL);
        return shader;
    }

    WeakPointer<Shader> GraphicsGL::createShader(const char vertex[], const char fragment[]) {
        std::shared_ptr<ShaderGL> shaderGL(new ShaderGL(vertex, fragment));
        shaders.push_back(shaderGL);
        std::shared_ptr<Shader> shader = std::static_pointer_cast<Shader>(shaderGL);
        return shader;
    }

    void GraphicsGL::activateShader(WeakPointer<Shader> shader) {
        glUseProgram(shader->getProgram());
    }

    std::shared_ptr<AttributeArrayGPUStorage> GraphicsGL::createGPUStorage(UInt32 size, UInt32 componentCount, AttributeType type, Bool normalize) const {
        AttributeArrayGPUStorageGL* gpuStorage =
            new (std::nothrow) AttributeArrayGPUStorageGL(size, componentCount, convertAttributeType(type), normalize ? GL_TRUE : GL_FALSE, 0);
        if (gpuStorage == nullptr) {
            throw AllocationException("GraphicsGL::createGPUStorage() -> Unable to allocate gpu buffer.");
        }
        std::shared_ptr<AttributeArrayGPUStorageGL> gpuStoragePtr(gpuStorage);
        return gpuStoragePtr;
    }

    std::shared_ptr<IndexBuffer> GraphicsGL::createIndexBuffer(UInt32 size) const {
        IndexBufferGL* indexBuffer = new (std::nothrow) IndexBufferGL(size);
        if (indexBuffer == nullptr) {
            throw AllocationException("GraphicsGL::createIndexBuffer() -> Unable to allocate index buffer.");
        }
        indexBuffer->initIndices();
        std::shared_ptr<IndexBuffer> bufferPtr(indexBuffer);
        return bufferPtr;
    }

    void GraphicsGL::drawBoundVertexBuffer(UInt32 vertexCount) {
        glPolygonMode(GL_FRONT_AND_BACK, getGLRenderStyle(this->renderStyle));
        glDrawArrays(GL_TRIANGLES, 0, vertexCount);
    }

    void GraphicsGL::drawBoundVertexBuffer(UInt32 vertexCount, WeakPointer<IndexBuffer> indices) {
        glPolygonMode(GL_FRONT_AND_BACK, getGLRenderStyle(this->renderStyle));
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices->getBufferID());
        glDrawElements(GL_TRIANGLES, vertexCount, GL_UNSIGNED_INT, (void*)(0));
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    ShaderManager& GraphicsGL::getShaderManager() {
        return this->shaderDirectory;
    }

    void GraphicsGL::setBlendingEnabled(Bool enabled) {
        if (enabled) glEnable(GL_BLEND);
        else glDisable(GL_BLEND);
    }

    void GraphicsGL::setBlendingFunction(RenderState::BlendingMethod source, RenderState::BlendingMethod dest) {
        glBlendFunc(getGLBlendProperty(source), getGLBlendProperty(dest));
    }

    WeakPointer<RenderTarget2D> GraphicsGL::createRenderTarget2D(Bool hasColor, Bool hasDepth, Bool enableStencilBuffer,
                                                                 const TextureAttributes& colorTextureAttributes, 
                                                                 const TextureAttributes& depthTextureAttributes, Vector2u size) {

        RenderTarget2DGL* renderTargetPtr = new(std::nothrow) RenderTarget2DGL(hasColor, hasDepth, enableStencilBuffer,
                                                                               colorTextureAttributes, depthTextureAttributes, size);
        if (renderTargetPtr == nullptr) {
            throw AllocationException("GraphicsGL::createRenderTarget2D -> Unable to allocate render target.");
        }
        std::shared_ptr<RenderTarget2DGL> target(renderTargetPtr);
        target->init();
        this->renderTarget2Ds.push_back(target);

        WeakPointer<RenderTarget2DGL> weakPtr = target;
        return weakPtr;
    }

    WeakPointer<RenderTargetCube> GraphicsGL::createRenderTargetCube(Bool hasColor, Bool hasDepth, Bool enableStencilBuffer,
                                                                     const TextureAttributes& colorTextureAttributes,
                                                                     const TextureAttributes& depthTextureAttributes, Vector2u size) {
       
        RenderTargetCubeGL* renderTargetPtr = new(std::nothrow) RenderTargetCubeGL(hasColor, hasDepth, enableStencilBuffer,
                                                                                   colorTextureAttributes, depthTextureAttributes, size);
        if (renderTargetPtr == nullptr) {
            throw AllocationException("GraphicsGL::createRenderTargetCube -> Unable to allocate render target.");
        }
        std::shared_ptr<RenderTargetCubeGL> target(renderTargetPtr);
        target->init();
        this->renderTargetCubes.push_back(target);

        WeakPointer<RenderTargetCubeGL> weakPtr = target;
        return weakPtr;
    }

    void GraphicsGL::setClearColor(Color color) {
        glClearColor(color.r, color.g, color.b, color.a);
    }

    void GraphicsGL::clearActiveRenderTarget(Bool colorBuffer, Bool depthBuffer, Bool stencilBuffer) {
        UInt32 mask = 0x00000000;
        if (colorBuffer) mask |= GL_COLOR_BUFFER_BIT;
        if (depthBuffer) mask |= GL_DEPTH_BUFFER_BIT;
        if (stencilBuffer) mask |= GL_STENCIL_BUFFER_BIT;
        glClear(mask);
    }

    WeakPointer<RenderTarget> GraphicsGL::getDefaultRenderTarget() {
        return this->defaultRenderTarget;
    }

    WeakPointer<RenderTarget> GraphicsGL::getCurrentRenderTarget() {
        return this->currentRenderTarget;
    }

    Bool GraphicsGL::activateRenderTarget(WeakPointer<RenderTarget> target) {
        if (!target.isValid()) {
            throw NullPointerException("GraphicsGL::activateRenderTarget -> 'target' is not valid.");
        }

        RenderTarget * renderTarget = target.get();
        RenderTargetGL * renderTargetGL = dynamic_cast<RenderTargetGL *>(renderTarget);
       
        if (renderTargetGL == nullptr) {
            throw InvalidArgumentException("GraphicsGL::activateRenderTarget -> Render target is not a valid OpenGL render target.");
        }

        if (this->currentRenderTarget.isValid()) {

            // prevent activating the currently active target.
            RenderTarget * currentTarget = this->currentRenderTarget.get();
            RenderTargetGL * currentRenderTargetGL = dynamic_cast<RenderTargetGL *>(currentTarget);

            if (currentRenderTargetGL != nullptr && currentRenderTargetGL->getFBOID() == renderTargetGL->getFBOID())return true;
        }

        glBindFramebuffer(GL_FRAMEBUFFER, renderTargetGL->getFBOID());
        this->currentRenderTarget = target;

        //GetCurrentBufferBits();

        return true;
    }

    Bool GraphicsGL::activateCubeRenderTargetSide(CubeTextureSide side) {
        if (this->currentRenderTarget.isValid()) {
            RenderTargetCubeGL * currentTargetCubeGL = dynamic_cast<RenderTargetCubeGL *>(this->currentRenderTarget.get());
            if (currentTargetCubeGL == nullptr) {
                throw Exception("GraphicsGL::activateCubeRenderTargetSide -> Current render target is not a valid OpenGL render target.");
            }

            CubeTextureGL * texGL = dynamic_cast<CubeTextureGL*>(currentTargetCubeGL->getColorTexture().get());
            if (texGL == nullptr) {
                throw InvalidArgumentException("GraphicsGL::activateCubeRenderTargetSide -> Render target texture is not a valid OpenGL texture.");
            }

            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, getGLCubeTarget(side), texGL->getTextureID(), 0);

            return true;
        }

        return false;
    }

    void GraphicsGL::updateDefaultRenderTargetSize(Vector2u size) {
        this->defaultRenderTarget->size = size;
    }

    void GraphicsGL::updateDefaultRenderTargetViewport(Vector4u viewport) {
        this->defaultRenderTarget->viewport = viewport;
    }

    void GraphicsGL::setRenderingToBufferEnabled(RenderBufferType type, Bool enabled) {
        switch(type) {
            case RenderBufferType::Color:
                if (enabled) {
                    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
                }
                else {
                    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
                }
            break;
            case RenderBufferType::Depth:
                if (enabled) {
                    glDepthMask(GL_TRUE);
                }
                else {
                    glDepthMask(GL_FALSE);
                }
            break;
            case RenderBufferType::Stencil:
                if (enabled) {
                    glStencilMask(0xFFFFFFFF);
                }
                else {
                    glStencilMask(0x00000000);
                }
            break;
        }
    }

    void GraphicsGL::setRenderStyle(RenderStyle style) {
        this->renderStyle = style;
    }

    void GraphicsGL::setDepthTestEnabled(Bool enabled) {
        if (enabled) glEnable(GL_DEPTH_TEST);
        else glDisable (GL_DEPTH_TEST);
    }

    void GraphicsGL::saveState() {
        glGetIntegerv(GL_FRONT_FACE, &this->_stateFrontFace);
        glGetBooleanv(GL_CULL_FACE, &this->_stateCullFaceEnabled);
        glGetIntegerv(GL_CULL_FACE_MODE, &this->_stateCullFaceMode);
        glGetBooleanv(GL_DEPTH_TEST, &this->_stateDepthTestEnabled);
        glGetIntegerv(GL_DEPTH_WRITEMASK, &this->_stateDepthMask);
        glGetIntegerv(GL_DEPTH_FUNC, &this->_stateDepthFunc);
        glGetBooleanv(GL_BLEND, &this->_stateBlendEnabled);
        glGetBooleanv(GL_LINE_SMOOTH, &this->_stateLineSmoothEnabled);
        glGetFloatv(GL_LINE_WIDTH, &this->_stateLineWidth);
        glGetIntegerv(GL_POLYGON_MODE, this->_statePolygonMode);
    }

    void GraphicsGL::restoreState() {
        glFrontFace(this->_stateFrontFace);
        glCullFace(this->_stateCullFaceMode);
        if (this->_stateCullFaceEnabled) glEnable(GL_CULL_FACE);
        else glDisable(GL_CULL_FACE);
        if (this->_stateDepthTestEnabled) glEnable(GL_DEPTH_TEST);
        else glDisable(GL_DEPTH_TEST);
        glDepthMask(this->_stateDepthMask);
        glDepthFunc(this->_stateDepthFunc);
        if (this->_stateBlendEnabled) glEnable(GL_BLEND);
        else glDisable(GL_BLEND);
        if (this->_stateLineSmoothEnabled) glEnable(GL_LINE_SMOOTH);
        else glDisable(GL_LINE_SMOOTH);
        glLineWidth(this->_stateLineWidth);
        glPolygonMode(GL_FRONT, this->_statePolygonMode[0]);
        glPolygonMode(GL_BACK, this->_statePolygonMode[1]);
    }

    /*
     * Get the OpenGL constant for texture cube side that corresponds
     * to [side].
     */
    GLenum GraphicsGL::getGLCubeTarget(CubeTextureSide side) {
        GLenum target;
        switch (side) {
            case CubeTextureSide::Back:
            target = GL_TEXTURE_CUBE_MAP_NEGATIVE_Z;
            break;
            case CubeTextureSide::Front:
            target = GL_TEXTURE_CUBE_MAP_POSITIVE_Z;
            break;
            case CubeTextureSide::Top:
            target = GL_TEXTURE_CUBE_MAP_POSITIVE_Y;
            break;
            case CubeTextureSide::Bottom:
            target = GL_TEXTURE_CUBE_MAP_NEGATIVE_Y;
            break;
            case CubeTextureSide::Left:
            target = GL_TEXTURE_CUBE_MAP_NEGATIVE_X;
            break;
            case CubeTextureSide::Right:
            target = GL_TEXTURE_CUBE_MAP_POSITIVE_X;
            break;
        }

        return target;
    }

    GLenum GraphicsGL::getGLBlendProperty(RenderState::BlendingMethod property) {
        switch (property) {
            case RenderState::BlendingMethod::SrcAlpha:
                return GL_SRC_ALPHA;
            case RenderState::BlendingMethod::OneMinusSrcAlpha:
                return GL_ONE_MINUS_SRC_ALPHA;
            case RenderState::BlendingMethod::DstAlpha:
                return GL_DST_ALPHA;
            case RenderState::BlendingMethod::OneMinusDstAlpha:
                return GL_ONE_MINUS_DST_ALPHA;
            case RenderState::BlendingMethod::One:
                return GL_ONE;
            case RenderState::BlendingMethod::Zero:
                return GL_ZERO;
            case RenderState::BlendingMethod::DstColor:
                return GL_DST_COLOR;
            case RenderState::BlendingMethod::OneMinusDstColor:
                return GL_ONE_MINUS_DST_COLOR;
            case RenderState::BlendingMethod::SrcColor:
                return GL_SRC_COLOR;
            case RenderState::BlendingMethod::OneMinusSrcColor:
                return GL_ONE_MINUS_SRC_COLOR;
            default:
                return (GLenum)0xFFFFFFFF;
        }
        return (GLenum)0xFFFFFFFF;
    }

    GLuint GraphicsGL::convertAttributeType(AttributeType type) {
        switch (type) {
            case AttributeType::Float:
                return GL_FLOAT;
            case AttributeType::UnsignedInt:
                return GL_UNSIGNED_INT;
            case AttributeType::Int:
                return GL_INT;
        }
        return 0;
    }

     /*
     * Get the OpenGL texture format that corresponds to [format].
     */
    GLint GraphicsGL::getGLTextureFormat(TextureFormat format) {
        switch (format) {
            case TextureFormat::R32F:
                return GL_R32F;
            case TextureFormat::RGBA8:
                return GL_RGBA8;
            case TextureFormat::RGBA16F:
                return GL_RGBA16F;
            case TextureFormat::RGBA32F:
                return GL_RGBA32F;

        }

        return GL_RGBA8;
    }

    /*
     * Get the OpenGL pixel format that corresponds to [format].
     */
    GLenum GraphicsGL::getGLPixelFormat(TextureFormat format) {
        switch (format) {
            case TextureFormat::R32F:
                return GL_RED;
            case TextureFormat::RGBA8:
                return GL_RGBA;
            case TextureFormat::RGBA16F:
                return GL_RGBA;
            case TextureFormat::RGBA32F:
                return GL_RGBA;

        }

        return GL_RGBA;
    }

    /*
     * Get the OpenGL pixel type that corresponds to [format].
     */
    GLenum GraphicsGL::getGLPixelType(TextureFormat format) {
        switch (format) {
            case TextureFormat::R32F:
                return GL_FLOAT;
            case TextureFormat::RGBA8:
                return GL_UNSIGNED_BYTE;
            case TextureFormat::RGBA16F:
                return GL_FLOAT;
            case TextureFormat::RGBA32F:
                return GL_FLOAT;

        }

        return GL_UNSIGNED_BYTE;
    }

    GLenum GraphicsGL::getGLRenderStyle(RenderStyle style) {
        switch(style) {
            case RenderStyle::Fill:
                return GL_FILL;
            case RenderStyle::Point:
                return GL_POINT;
            case RenderStyle::Line:
                return GL_LINE;
        }
    }

    std::shared_ptr<RendererGL> GraphicsGL::createRenderer() {
        RendererGL* renderPtr = new RendererGL();
        if (renderPtr == nullptr) {
            throw AllocationException("GraphicsGL::createRenderer -> Unable to allocate renderer.");
        }
        std::shared_ptr<RendererGL> renderer(renderPtr);
        renderer->init();
        return renderer;
    }

    std::shared_ptr<RenderTarget2DGL> GraphicsGL::createDefaultRenderTarget() {
        TextureAttributes colorAttributes;
        TextureAttributes depthAttributes;
        Vector2u renderSize(1024, 1024);
        RenderTarget2DGL* defaultTargetPtr = new(std::nothrow) RenderTarget2DGL(false, false, false, colorAttributes,
                                                                                depthAttributes, renderSize);
        if (defaultTargetPtr == nullptr) {
            throw AllocationException("GraphicsGL::createDefaultRenderTarget -> Unable to allocate default render target.");
        }
        std::shared_ptr<RenderTarget2DGL> defaultTarget(defaultTargetPtr);
        this->renderTarget2Ds.push_back(defaultTarget);
        return defaultTarget;
    }
}