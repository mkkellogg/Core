#include "GraphicsGL.h"

namespace Core {

    static void printGlString(const char* name, GLenum s) {
        const char* v = (const char*)glGetString(s);
        Debug::PrintMessage("GL %s: %s\n", name, v);
    }

    GraphicsGL::GraphicsGL(GLVersion version): glVersion(version) {

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

        renderer = std::shared_ptr<RendererGL>(RendererGL::createRenderer());

        glClearColor(0, 0, 0, 1);
        glFrontFace(GL_CCW);
        glCullFace(GL_BACK);
        glEnable(GL_CULL_FACE);
        glEnable(GL_DEPTH_TEST);
        glDepthMask(GL_TRUE);
        glDepthFunc(GL_LEQUAL);

    }

    std::weak_ptr<Renderer> GraphicsGL::getRenderer() {
        return std::static_pointer_cast<Renderer>(this->renderer);
    }

    std::weak_ptr<Texture2D> GraphicsGL::createTexture2D() {
        std::shared_ptr<Texture2DGL> newTexture = std::shared_ptr<Texture2DGL>(new Texture2DGL());
        this->textures2D.push_back(newTexture);
        return std::static_pointer_cast<Texture2D>(newTexture);
    }

    std::weak_ptr<CubeTexture> GraphicsGL::createCubeTexture() {
        std::shared_ptr<CubeTextureGL> newTexture = std::shared_ptr<CubeTextureGL>(new CubeTextureGL());
        this->cubeTextures.push_back(newTexture);
        return std::static_pointer_cast<CubeTexture>(newTexture);
    }

    std::weak_ptr<Mesh> GraphicsGL::createMesh(UInt32 size, Bool indexed) {
        std::shared_ptr<MeshGL> newMesh = std::shared_ptr<MeshGL>(new MeshGL(size, indexed));
        this->meshes.push_back(newMesh);
        return std::static_pointer_cast<Mesh>(newMesh);
    }

    std::shared_ptr<AttributeArrayGPUStorage> GraphicsGL::createGPUStorage(UInt32 size, UInt32 componentCount, AttributeType type, Bool normalize) const {
         AttributeArrayGPUStorageGL* gpuStorage = new (std::nothrow) AttributeArrayGPUStorageGL(size, componentCount, convertAttributeType(type), normalize ? GL_TRUE : GL_FALSE, 0);
        if (gpuStorage == nullptr) {
            throw AllocationException("GraphicsGL::createGPUStorage() -> Unable to allocate gpu buffer.");
        } 
        std::shared_ptr<AttributeArrayGPUStorageGL> gpuStoragePtr(gpuStorage);
        return gpuStoragePtr;
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
    }

}