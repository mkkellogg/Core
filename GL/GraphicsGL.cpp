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

    std::weak_ptr<Texture> GraphicsGL::createTexture() {
        std::shared_ptr<Texture2DGL> newTexture = std::shared_ptr<Texture2DGL>(new Texture2DGL());
        this->textures.push_back(newTexture);
        return std::static_pointer_cast<Texture>(newTexture);
    }

}