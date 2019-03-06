#include <memory>
#include <vector>

#include "CubeTextureGL.h"
#include "GraphicsGL.h"
#include "../Engine.h"
#include "../common/Exception.h"
#include "../common/gl.h"
#include "../image/RawImage.h"

namespace Core {

    CubeTextureGL::CubeTextureGL(const TextureAttributes& attributes): CubeTexture(attributes) {

    }

    CubeTextureGL::~CubeTextureGL() {
        
    }

    void CubeTextureGL::build(WeakPointer<StandardImage> front, WeakPointer<StandardImage> back, 
                              WeakPointer<StandardImage> top, WeakPointer<StandardImage> bottom, 
                              WeakPointer<StandardImage> left, WeakPointer<StandardImage> right) {
                                  
        this->setupTexture(front->getWidth(), front->getHeight(), 
                           front->getImageData(), back->getImageData(), 
                           top->getImageData(), bottom->getImageData(), 
                           left->getImageData(), right->getImageData());
    }

    void CubeTextureGL::build(UInt32 width, UInt32 height) {
        this->setupTexture(width, height, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
    }

    void CubeTextureGL::setupTexture(UInt32 width, UInt32 height, Byte* front, Byte* back, Byte* top, Byte* bottom, Byte* left, Byte* right) {
        WeakPointer<Graphics> graphics = Engine::instance()->getGraphicsSystem();
        WeakPointer<GraphicsGL> graphicsGL =  WeakPointer<Graphics>::dynamicPointerCast<GraphicsGL>(graphics);
        
        glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

        GLuint tex;

        // generate the OpenGL cube texture
        glGenTextures(1, &tex);
        if (!tex) {
            throw AllocationException("CubeTexture::createCubeTexture -> Unable to generate texture");
        }
        glBindTexture(GL_TEXTURE_CUBE_MAP, tex);

        GLvoid * frontPixels = front != nullptr ? front : (GLvoid*)0;
        GLvoid * backPixels = back != nullptr ? back : (GLvoid*)0;
        GLvoid * topPixels = top != nullptr ? top : (GLvoid*)0;
        GLvoid * bottomPixels = bottom != nullptr ? bottom : (GLvoid*)0;
        GLvoid * leftPixels = left != nullptr ? left : (GLvoid*)0;
        GLvoid * rightPixels = right != nullptr ? right : (GLvoid*)0;

        std::vector<GLvoid*> images = {frontPixels, backPixels, topPixels, bottomPixels, leftPixels, rightPixels};
        std::vector<GLuint> faces = {GL_TEXTURE_CUBE_MAP_POSITIVE_Z, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,
                                     GL_TEXTURE_CUBE_MAP_POSITIVE_Y, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
                                     GL_TEXTURE_CUBE_MAP_NEGATIVE_X, GL_TEXTURE_CUBE_MAP_POSITIVE_X};

        GLint textureFormat = graphicsGL->getGLTextureFormat(attributes.Format);
        GLenum pixelFormat = graphicsGL->getGLPixelFormat(attributes.Format);
        GLenum pixelType = graphicsGL->getGLPixelType(attributes.Format);

        for (UInt32 i = 0; i < 6; i++) {
             if (attributes.IsDepthTexture) {
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
                glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE, GL_INTENSITY);
                glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
            }
            else {
                glTexImage2D(faces[i], 0, textureFormat, width, height, 0, pixelFormat, pixelType, images[i]);
            }
        }

        // set the relevant texture properties
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

        glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

        this->textureId = (Int32)tex;
    }
}