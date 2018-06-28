#include "Texture2DGL.h"
#include "../common/Exception.h"
#include "../image/RawImage.h"

namespace Core {

    Texture2DGL::Texture2DGL(const TextureAttributes& attributes): Texture2D(attributes) {

    }

    Texture2DGL::~Texture2DGL() {

    }

    void Texture2DGL::build(RawImage *imageData) {
        GLuint tex;

        // generate the OpenGL texture
        glGenTextures(1, &tex);
        if (!tex) {
            throw AllocationException("Texture2DGL::createTexture -> Unable to generate texture");
        }
        glBindTexture(GL_TEXTURE_2D, tex);

        // set the wrap mode
        if (attributes.WrapMode == TextureWrap::Mirror) {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
        }
        else if (attributes.WrapMode == TextureWrap::Repeat) {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        }
        else {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        }

        // set the filter mode. if bi-linear or tri-linear filtering is used,
        // we will be using mip-maps
        if (this->attributes.FilterMode == TextureFilter::Linear) {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        }
        else if (this->attributes.FilterMode == TextureFilter::BiLinear) {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        }
        else if (this->attributes.FilterMode == TextureFilter::TriLinear) {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        }
        else {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        }

        // we only generate mip-maps if bi-linear or tri-linear filtering is used
        if (attributes.FilterMode == TextureFilter::TriLinear || attributes.FilterMode == TextureFilter::BiLinear) {
            glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, attributes.MipMapLevel);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, attributes.MipMapLevel);
        }

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageData->getWidth(), imageData->getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData->getImageData());

        glBindTexture(GL_TEXTURE_2D, 0);
        this->textureId = (Int32)tex;
    }
    
};
