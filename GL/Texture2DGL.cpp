#include "Texture2DGL.h"
#include "../common/Exception.h"

namespace Core {

    Texture2DGL::Texture2DGL(const TextureAttributes& attributes): Texture2D(attributes) {

    }

    Texture2DGL::~Texture2DGL() {

    }

    void Texture2DGL::build(RawImage *imageData) {
        // glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

        GLuint tex;

        // generate the OpenGL cube texture
        glGenTextures(1, &tex);
        if (!tex) {
            throw AllocationException("Texture::createTexture -> Unable to generate texture");
        }
        glBindTexture(GL_TEXTURE_2D, tex);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageData->getWidth(), imageData->getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData->getImageData());

        // set the relevant texture properties
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

        glBindTexture(GL_TEXTURE_2D, 0);
        this->textureId = (Int32)tex;
    }
    
};
