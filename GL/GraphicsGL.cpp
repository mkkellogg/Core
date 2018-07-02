#include <algorithm>

#include "GraphicsGL.h"
#include "AttributeArrayGPUStorageGL.h"
#include "CubeTextureGL.h"
#include "IndexBufferGL.h"
#include "RendererGL.h"
#include "ShaderGL.h"
#include "Texture2DGL.h"

namespace Core {

    static void printGlString(const char* name, GLenum s) {
        const char* v = (const char*)glGetString(s);
        Debug::PrintMessage("GL %s: %s\n", name, v);
    }

    GraphicsGL::GraphicsGL(GLVersion version) : glVersion(version) {
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

        renderer = std::shared_ptr<RendererGL>(new RendererGL());
        renderer->init();

        this->shaderDirectory.init();
    }

    WeakPointer<Renderer> GraphicsGL::getRenderer() {
        return std::static_pointer_cast<Renderer>(this->renderer);
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
        glDrawArrays(GL_TRIANGLES, 0, vertexCount);
    }

    void GraphicsGL::drawBoundVertexBuffer(UInt32 vertexCount, WeakPointer<IndexBuffer> indices) {
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
}