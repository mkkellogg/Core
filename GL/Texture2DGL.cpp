#include "Texture2DGL.h"
#include "GraphicsGL.h"
#include "../Engine.h"
#include "../common/Exception.h"
#include "../image/RawImage.h"

namespace Core {

    Texture2DGL::Texture2DGL(const TextureAttributes& attributes): Texture2D(attributes) {

    }

    Texture2DGL::~Texture2DGL() {
        if (this->textureId > 0) {
            glDeleteTextures(1, &this->textureId);
        }
    }

    void Texture2DGL::build(WeakPointer<RawImage> imageData) {
        this->setupTexture(imageData->getWidth(), imageData->getHeight(), imageData->getImageData());
    }
      
    void Texture2DGL::build(UInt32 width, UInt32 height) {
        this->setupTexture(width, height, nullptr);
    }

    void Texture2DGL::setupTexture(UInt32 width, UInt32 height, Byte* data) {
        WeakPointer<Graphics> graphics = Engine::instance()->getGraphicsSystem();
        WeakPointer<GraphicsGL> graphicsGL =  WeakPointer<Graphics>::dynamicPointerCast<GraphicsGL>(graphics);

        GLuint tex;
        
        // generate the OpenGL texture
        glGenTextures(1, &tex);
        if (!tex) {
            throw AllocationException("Texture2DGL::setupTexture -> Unable to generate texture");
        }
        glBindTexture(GL_TEXTURE_2D, tex);

        // set the wrap mode
        if (this->attributes.WrapMode == TextureWrap::Mirror) {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
        }
        else if (this->attributes.WrapMode == TextureWrap::Repeat) {
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
        if (this->attributes.FilterMode == TextureFilter::TriLinear || attributes.FilterMode == TextureFilter::BiLinear) {
            glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, attributes.MipMapLevel);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, attributes.MipMapLevel);
        }

        GLenum textureFormat = graphicsGL->getGLTextureFormat(attributes.Format);
        GLenum pixelFormat = graphicsGL->getGLPixelFormat(attributes.Format);
        GLenum pixelType = graphicsGL->getGLPixelType(attributes.Format);

        if (attributes.IsDepthTexture) {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
           // glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE, GL_INTENSITY);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
        }
        else {
            glTexImage2D(GL_TEXTURE_2D, 0, textureFormat, width, height, 0, pixelFormat, pixelType, data);
        }
       
        glBindTexture(GL_TEXTURE_2D, 0);
        this->textureId = (Int32)tex;
    }
    
};
