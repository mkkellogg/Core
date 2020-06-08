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
        Graphics::init();

        printGlString("Version", GL_VERSION);
        printGlString("Vendor", GL_VENDOR);
        printGlString("Renderer", GL_RENDERER);
        printGlString("Extensions", GL_EXTENSIONS);

        UInt32 maxGL = 0;
        const char* versionStr = (const char*)glGetString(GL_VERSION);
        this->defaultRenderTarget = this->createDefaultRenderTarget();
        this->currentRenderTarget = this->defaultRenderTarget;
        this->shaderDirectory.init();

        this->renderer = this->createRenderer();
        if (!this->sharedRenderState) {
            this->setupRenderState();
        }
        
        glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
    }

    WeakPointer<Renderer> GraphicsGL::getRenderer() {
        return std::static_pointer_cast<Renderer>(this->renderer);
    }

    void GraphicsGL::preRender() {
        if (!this->sharedRenderState) {
            this->saveState();
            this->setupRenderState();
        }
    }

    void GraphicsGL::postRender() {
        if (!this->sharedRenderState) {
            this->restoreState();
        }
    }

    WeakPointer<Texture2D> GraphicsGL::createTexture2D(const TextureAttributes& attributes) {
        Texture2DGL* newTexturePtr = new(std::nothrow) Texture2DGL(attributes);
        if (newTexturePtr == nullptr) {
            throw AllocationException("GraphicsGL::createTexture2D -> Unable to allocate new Texture2DGL");
        }
        std::shared_ptr<Texture2DGL> newTexture = std::shared_ptr<Texture2DGL>(newTexturePtr);
        this->addCoreObjectReference(newTexture, CoreObjectReferenceManager::OwnerType::Single);this->addCoreObjectReference(newTexture, CoreObjectReferenceManager::OwnerType::Single);
        return std::static_pointer_cast<Texture2D>(newTexture);
    }

    WeakPointer<CubeTexture> GraphicsGL::createCubeTexture(const TextureAttributes& attributes) {
        CubeTextureGL* newTexturePtr = new(std::nothrow) CubeTextureGL(attributes);
        if (newTexturePtr == nullptr) {
            throw AllocationException("GraphicsGL::createCubeTexture -> Unable to allocate new CubeTextureGL");
        }
        std::shared_ptr<CubeTextureGL> newTexture = std::shared_ptr<CubeTextureGL>(newTexturePtr);
       this->addCoreObjectReference(newTexture, CoreObjectReferenceManager::OwnerType::Single);
        return std::static_pointer_cast<CubeTexture>(newTexture);
    }

    WeakPointer<Shader> GraphicsGL::createShader(const std::string& vertex, const std::string& fragment) {
        ShaderGL* shaderPtr = new(std::nothrow) ShaderGL(vertex, fragment);
        return this->addShader(shaderPtr);
    }

    WeakPointer<Shader> GraphicsGL::createShader(const std::string& vertex, const std::string& geometry, const std::string& fragment) {
        ShaderGL* shaderPtr = new(std::nothrow) ShaderGL(vertex, geometry, fragment);
        return this->addShader(shaderPtr);
    }

    WeakPointer<Shader> GraphicsGL::createShader(const char vertex[], const char fragment[]) {
        ShaderGL* shaderPtr = new(std::nothrow) ShaderGL(vertex, fragment);
        return this->addShader(shaderPtr);
    }

    WeakPointer<Shader> GraphicsGL::createShader(const char vertex[], const char geometry[], const char fragment[]) {
        ShaderGL* shaderPtr = new(std::nothrow) ShaderGL(vertex, geometry, fragment);
        return this->addShader(shaderPtr);
    }

    WeakPointer<Shader> GraphicsGL::addShader(ShaderGL* shaderPtr) {
        if (shaderPtr == nullptr) {
            throw AllocationException("GraphicsGL::addShader -> Could not allocate new shader.");
        }
        std::shared_ptr<ShaderGL> spShaderGL(shaderPtr);
        this->addCoreObjectReference(spShaderGL, CoreObjectReferenceManager::OwnerType::Single);
        std::shared_ptr<Shader> spShader = std::static_pointer_cast<Shader>(spShaderGL);
        return spShader;
    }

    void GraphicsGL::activateShader(WeakPointer<Shader> shader) {
        glUseProgram(shader->getProgram());
    }

    std::shared_ptr<AttributeArrayGPUStorage> GraphicsGL::createGPUStorage(UInt32 size, UInt32 componentCount, AttributeType type, Bool normalize) {
        AttributeArrayGPUStorageGL* gpuStoragePtr =
            new (std::nothrow) AttributeArrayGPUStorageGL(size, componentCount, convertAttributeType(type), normalize ? GL_TRUE : GL_FALSE, 0);
        if (gpuStoragePtr == nullptr) {
            throw AllocationException("GraphicsGL::createGPUStorage() -> Unable to allocate gpu buffer.");
        }
        std::shared_ptr<AttributeArrayGPUStorageGL> spGpuStorage(gpuStoragePtr);
        return spGpuStorage;
    }

    std::shared_ptr<IndexBuffer> GraphicsGL::createIndexBuffer(UInt32 size) {
        IndexBufferGL* indexBufferPtr = new (std::nothrow) IndexBufferGL(size);
        if (indexBufferPtr == nullptr) {
            throw AllocationException("GraphicsGL::createIndexBuffer() -> Unable to allocate index buffer.");
        }
        indexBufferPtr->initIndices();
        std::shared_ptr<IndexBufferGL> spIndexBuffer(indexBufferPtr);
        return spIndexBuffer;
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

    void GraphicsGL::setBlendingEquation(RenderState::BlendingEquation equation) {
        glBlendEquation(getGLBlendingEquation(equation));
    }

    void GraphicsGL::setBlendingFactors(RenderState::BlendingFactor source, RenderState::BlendingFactor dest) {
        glBlendFunc(getGLBlendingFactor(source), getGLBlendingFactor(dest));
    }

    WeakPointer<RenderTarget2D> GraphicsGL::createRenderTarget2D(Bool hasColor, Bool hasDepth, Bool enableStencilBuffer,
                                                                 const TextureAttributes& colorTextureAttributes, 
                                                                 const TextureAttributes& depthTextureAttributes,
                                                                 const Vector2u& size) {

        RenderTarget2DGL* renderTargetPtr = new(std::nothrow) RenderTarget2DGL(hasColor, hasDepth, enableStencilBuffer,
                                                                               colorTextureAttributes, depthTextureAttributes, size);
        if (renderTargetPtr == nullptr) {
            throw AllocationException("GraphicsGL::createRenderTarget2D -> Unable to allocate render target.");
        }
        std::shared_ptr<RenderTarget2DGL> target(renderTargetPtr);
        target->init();
        this->addCoreObjectReference(target, CoreObjectReferenceManager::OwnerType::Single);

        WeakPointer<RenderTarget2DGL> weakPtr = target;
        return weakPtr;
    }

    WeakPointer<RenderTargetCube> GraphicsGL::createRenderTargetCube(Bool hasColor, Bool hasDepth, Bool enableStencilBuffer,
                                                                     const TextureAttributes& colorTextureAttributes,
                                                                     const TextureAttributes& depthTextureAttributes, const Vector2u& size) {
       
        RenderTargetCubeGL* renderTargetPtr = new(std::nothrow) RenderTargetCubeGL(hasColor, hasDepth, enableStencilBuffer,
                                                                                   colorTextureAttributes, depthTextureAttributes, size);
        if (renderTargetPtr == nullptr) {
            throw AllocationException("GraphicsGL::createRenderTargetCube -> Unable to allocate render target.");
        }
        std::shared_ptr<RenderTargetCubeGL> target(renderTargetPtr);
        target->init();
        this->addCoreObjectReference(target, CoreObjectReferenceManager::OwnerType::Single);

        WeakPointer<RenderTargetCubeGL> weakPtr = target;
        return weakPtr;
    }

    void GraphicsGL::setColorWriteEnabled(Bool enabled) {
        if (enabled) {
            glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
        }
        else {
            glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
        }
    }

    void GraphicsGL::setClearColor(Color color) {
        glClearColor(color.r, color.g, color.b, color.a);
    }

    void GraphicsGL::clearActiveRenderTarget(Bool colorBuffer, Bool depthBuffer, Bool stencilBuffer) {
        UInt32 mask = 0x00000000;
        if (colorBuffer) mask |= GL_COLOR_BUFFER_BIT;
        if (depthBuffer) mask |= GL_DEPTH_BUFFER_BIT;
        if (stencilBuffer) mask |= GL_STENCIL_BUFFER_BIT;

        if (colorBuffer) glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
        if (depthBuffer) glDepthMask(GL_TRUE);
        if (stencilBuffer) glStencilMask(0xFF);

        glClear(mask);
    }

    void GraphicsGL::setDefaultRenderTargetToCurrent() {
        GLint initialDrawFBOID =  -1;
        glGetIntegerv(GL_FRAMEBUFFER_BINDING, &initialDrawFBOID);
        GLint initialReadFBOID =  -1;
        glGetIntegerv(GL_READ_FRAMEBUFFER_BINDING, &initialReadFBOID);
        this->defaultRenderTarget->setFBOID(initialDrawFBOID);
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

    Bool GraphicsGL::activateRenderTarget2DMipLevel(UInt32 mipLevel) {
        if (this->currentRenderTarget.isValid()) {
            RenderTarget2DGL * currentTarget2DGL = dynamic_cast<RenderTarget2DGL *>(this->currentRenderTarget.get());
            if (currentTarget2DGL == nullptr) {
                throw Exception("GraphicsGL::activateRenderTarget2DMipLevel -> Current render target is not a valid OpenGL render target.");
            }

            for (UInt32 i = 0; i < currentTarget2DGL->getColorTextureCount(); i++) {
                WeakPointer<Texture> colorTexture = currentTarget2DGL->getColorTexture(i);
                if (colorTexture.isValid()) {
                    Texture2DGL * texGL = dynamic_cast<Texture2DGL*>(colorTexture.get());
                    if (texGL == nullptr) {
                        throw InvalidArgumentException("GraphicsGL::activateRenderTarget2DMipLevel -> Render target texture is not a valid OpenGL texture.");
                    }

                    GLint attachment = GraphicsGL::getColorAttachmentID(i);
                    glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, texGL->getTextureID(), mipLevel);
                    return true;
                }
            }
            return false;            
        }

        return false;
    }

    Bool GraphicsGL::activateCubeRenderTargetSide(CubeTextureSide side, UInt32 mipLevel) {
        if (this->currentRenderTarget.isValid()) {
            RenderTargetCubeGL * currentTargetCubeGL = dynamic_cast<RenderTargetCubeGL *>(this->currentRenderTarget.get());
            if (currentTargetCubeGL == nullptr) {
                throw Exception("GraphicsGL::activateCubeRenderTargetSide -> Current render target is not a valid OpenGL render target.");
            }

            for (UInt32 i = 0; i < currentTargetCubeGL->getColorTextureCount(); i++) {
                CubeTextureGL * texGL = dynamic_cast<CubeTextureGL*>(currentTargetCubeGL->getColorTexture(i).get());
                if (texGL == nullptr) {
                    throw InvalidArgumentException("GraphicsGL::activateCubeRenderTargetSide -> Render target texture is not a valid OpenGL texture.");
                }

                GLint attachment = GraphicsGL::getColorAttachmentID(i);
                glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, getGLCubeTarget(side), texGL->getTextureID(), mipLevel);
            }

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

    Vector4u GraphicsGL::getViewport() {
        return this->_viewport;
    }

    void GraphicsGL::setViewport(UInt32 hOffset, UInt32 vOffset, UInt32 viewPortWidth, UInt32 viewPortHeight) {
        glViewport(hOffset, vOffset, viewPortWidth, viewPortHeight);
        this->_viewport.set(hOffset, vOffset, viewPortWidth, viewPortHeight);
    }

    void GraphicsGL::setRenderStyle(RenderStyle style) {
        this->renderStyle = style;
    }

    void GraphicsGL::setDepthWriteEnabled(Bool enabled) {
        if (enabled) {
            glDepthMask(GL_TRUE);
        }
        else {
            glDepthMask(GL_FALSE);
        }
    }

    void GraphicsGL::setDepthTestEnabled(Bool enabled) {
        if (enabled) glEnable(GL_DEPTH_TEST);
        else glDisable (GL_DEPTH_TEST);
    }

    void GraphicsGL::setDepthFunction(RenderState::DepthFunction function) {
        glDepthFunc(getGLDepthFunction(function));
    }

    void GraphicsGL::setStencilTestEnabled(Bool enabled) {
        if (enabled) {
            glEnable(GL_STENCIL_TEST);
        }
        else {
            glDisable(GL_STENCIL_TEST);
        }
    }

    void GraphicsGL::setStencilWriteMask(UInt32 mask) {
        glStencilMask((GLuint)mask);
    }

    void GraphicsGL::setStencilFunction(RenderState::StencilFunction function, Int16 value, UInt16 mask) {
        glStencilFunc(getGLStencilFunction(function), (GLint)value, (GLuint)mask);
    }

    void GraphicsGL::setStencilOperation(RenderState::StencilAction sFail, RenderState::StencilAction dpFail, RenderState::StencilAction dpPass) {
        glStencilOp(getGLStencilAction(sFail), getGLStencilAction(dpFail), getGLStencilAction(dpPass));
    }

    void GraphicsGL::setFaceCullingEnabled(Bool enabled) {
        if (enabled) {
            glEnable(GL_CULL_FACE);
        }
        else {
            glDisable(GL_CULL_FACE);
        }
    }

    void GraphicsGL::setCullFace(RenderState::CullFace face) {
        switch(face) {
            case RenderState::CullFace::Front:
                glCullFace(GL_FRONT);
            break;
            case RenderState::CullFace::Back:
                glCullFace(GL_BACK);
            break;
        }
    }

    void GraphicsGL::setRenderLineSize(Real size) {
        glLineWidth(size);
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

    void GraphicsGL::lowLevelBlit(WeakPointer<RenderTarget> source, WeakPointer<RenderTarget> destination, Int16 cubeFace, Bool includeColor, Bool includeDepth) {
        GLint srcID = (dynamic_cast<RenderTargetGL *>(source.get()))->getFBOID();
        GLint destID = (dynamic_cast<RenderTargetGL *>(destination.get()))->getFBOID();
        Vector2u size = destination->getSize();

        glBindFramebuffer(GL_READ_FRAMEBUFFER, srcID);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, destID);
        if (includeColor && cubeFace >= 0) {
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, getGLCubeTarget((CubeTextureSide)cubeFace), destination->getColorTexture()->getTextureID(), 0);

        }
        GLuint mask = 0;
        if (includeColor) mask |= GL_COLOR_BUFFER_BIT;
        if (includeDepth) mask |= GL_DEPTH_BUFFER_BIT;
        glBlitFramebuffer(0, 0, size.x, size.y, 0, 0, size.x, size.y, mask, GL_NEAREST);
    }

    /*
     * Set the test that is used when performing depth-buffer occlusion.
     */
    GLint GraphicsGL::getGLDepthFunction(RenderState::DepthFunction function) {
        switch (function) {
            case RenderState::DepthFunction::Always:
                return GL_ALWAYS;
            case RenderState::DepthFunction::Greater:
                return GL_GREATER;
            case RenderState::DepthFunction::GreaterThanOrEqual:
                return GL_GEQUAL;
            case RenderState::DepthFunction::Less:
                return GL_LESS;
            case RenderState::DepthFunction::LessThanOrEqual:
                return GL_LEQUAL;
            case RenderState::DepthFunction::Equal:
                return GL_EQUAL;
        }
        return -1;
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
            
    GLenum GraphicsGL::getGLBlendingEquation(RenderState::BlendingEquation equation) {
        switch (equation) {
            case RenderState::BlendingEquation::Add:
                return GL_FUNC_ADD;
            case RenderState::BlendingEquation::Subtract:
                return GL_FUNC_SUBTRACT;
            case RenderState::BlendingEquation::ReverseSubtract:
                return GL_FUNC_REVERSE_SUBTRACT;
            case RenderState::BlendingEquation::Min:
                return GL_MIN;
            case RenderState::BlendingEquation::Max:
                return GL_MAX;
            default:
                return (GLenum)0xFFFFFFFF;
        }
        return (GLenum)0xFFFFFFFF;
    }

    GLenum GraphicsGL::getGLBlendingFactor(RenderState::BlendingFactor factor) {
        switch (factor) {
            case RenderState::BlendingFactor::SrcAlpha:
                return GL_SRC_ALPHA;
            case RenderState::BlendingFactor::OneMinusSrcAlpha:
                return GL_ONE_MINUS_SRC_ALPHA;
            case RenderState::BlendingFactor::DstAlpha:
                return GL_DST_ALPHA;
            case RenderState::BlendingFactor::OneMinusDstAlpha:
                return GL_ONE_MINUS_DST_ALPHA;
            case RenderState::BlendingFactor::One:
                return GL_ONE;
            case RenderState::BlendingFactor::Zero:
                return GL_ZERO;
            case RenderState::BlendingFactor::DstColor:
                return GL_DST_COLOR;
            case RenderState::BlendingFactor::OneMinusDstColor:
                return GL_ONE_MINUS_DST_COLOR;
            case RenderState::BlendingFactor::SrcColor:
                return GL_SRC_COLOR;
            case RenderState::BlendingFactor::OneMinusSrcColor:
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
#ifdef GL_RGBA16F
                return GL_RGBA16F;
#else
                return GL_RGBA16F_ARB;
#endif
            case TextureFormat::RGBA32F:
#ifdef GL_RGBA32F
                return GL_RGBA32F;
#else
                return GL_RGBA32F_ARB;
#endif
            case TextureFormat::RG16F:
                return GL_RG16F;
            case TextureFormat::DEPTH16:
                return GL_DEPTH_COMPONENT16;
            case TextureFormat::DEPTH24:
                return GL_DEPTH_COMPONENT24;
            case TextureFormat::DEPTH32:
                return GL_DEPTH_COMPONENT32;

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
            case TextureFormat::RG16F:
                return GL_RG;
            case TextureFormat::DEPTH:
                return GL_DEPTH_COMPONENT;

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
            case TextureFormat::RG16F:
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
        return GL_FILL;
    }

    GLenum GraphicsGL::getGLStencilFunction(RenderState::StencilFunction function) {
        switch (function) {
            case RenderState::StencilFunction::Never:
                return GL_NEVER;
            break;
            case RenderState::StencilFunction::Less:
                return GL_LESS;
            break;
            case RenderState::StencilFunction::LessThanOrEqual:
                return GL_LEQUAL;
            break;
            case RenderState::StencilFunction::Greater:
                return GL_GREATER;
            break;
            case RenderState::StencilFunction::GreaterThanOrEqual:
                return GL_GEQUAL;
            break;
            case RenderState::StencilFunction::Equal:
                return GL_EQUAL;
            break;
            case RenderState::StencilFunction::NotEqual:
                return GL_NOTEQUAL;
            break;
            case RenderState::StencilFunction::Always:
                return GL_ALWAYS;
            break;
        }
        return GL_NEVER;
    }

    GLenum GraphicsGL::getGLStencilAction(RenderState::StencilAction action) {
        switch (action) {
            case RenderState::StencilAction::Keep:
                return GL_KEEP;
            break;
            case RenderState::StencilAction::Zero:
                return GL_ZERO;
            break;
            case RenderState::StencilAction::Replace:
                return GL_REPLACE;
            break;
            case RenderState::StencilAction::Increment:
                return GL_INCR;
            break;
            case RenderState::StencilAction::IncrementWrap:
                return GL_INCR_WRAP;
            break;
            case RenderState::StencilAction::Decrement:
                return GL_DECR;
            break;
            case RenderState::StencilAction::DecrementWrap:
                return GL_DECR_WRAP;
            break;
            case RenderState::StencilAction::Invert:
                return GL_INVERT;
            break;
        }
        return GL_KEEP;
    }

    GLint GraphicsGL::getColorAttachmentID(UInt32 index) {
        switch(index) {
            case 0:
                return GL_COLOR_ATTACHMENT0;
            case 1:
                return GL_COLOR_ATTACHMENT1;
            case 2: 
                return GL_COLOR_ATTACHMENT2;
            case 3:
                return GL_COLOR_ATTACHMENT3;
        }
        return -1;
    }

    std::shared_ptr<RendererGL> GraphicsGL::createRenderer() {
        RendererGL* renderPtr = new(std::nothrow) RendererGL();
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
                                                                                depthAttributes, renderSize, 0);
        if (defaultTargetPtr == nullptr) {
            throw AllocationException("GraphicsGL::createDefaultRenderTarget -> Unable to allocate default render target.");
        }
        std::shared_ptr<RenderTarget2DGL> defaultTarget(defaultTargetPtr);
        this->addCoreObjectReference(defaultTarget, CoreObjectReferenceManager::OwnerType::Single);
        return defaultTarget;
    }

    void GraphicsGL::setupRenderState() {
        // TODO: Move these state calls to a place where they are not called every frame
        glFrontFace(GL_CW);
        glCullFace(GL_BACK);
        glEnable(GL_CULL_FACE);
        glEnable(GL_DEPTH_TEST);
        glDepthMask(GL_TRUE);
        glDepthFunc(GL_LEQUAL);
        glDisable(GL_BLEND);

        glLineWidth(1.5);
        glEnable(GL_LINE_SMOOTH);
        glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    }
}