#pragma once

#include <string.h>
#include <new>

#include "../geometry/AttributeArrayGPUStorage.h"
#include "../common/types.h"
#include "../common/gl.h"

namespace Core {

    class AttributeArrayGPUStorageGL final: public AttributeArrayGPUStorage {
    public:
        AttributeArrayGPUStorageGL(UInt32 size, UInt32 componentCount, GLenum type, GLboolean normalize, GLsizei stride): 
            size(size), componentCount(componentCount), type(type), normalize(normalize), stride(stride) {
            buildGPUBuffer();
        }

        ~AttributeArrayGPUStorageGL() override{
            destroyGPUBuffer();
        }

        Int32 getBufferID() const override {
            return this->bufferID;
        }

        GLenum getType() const {
            return this->type;
        }

        GLboolean shouldNormalize() const {
            return this->normalize;
        }

        GLsizei getStride() const {
            return stride;
        }

        void enableAndSendToActiveShader(UInt32 location) override {
            glBindBuffer(GL_ARRAY_BUFFER, this->bufferID);
            glEnableVertexAttribArray(location);
            if (this->type == GL_INT || this->type == GL_UNSIGNED_INT) {
#ifdef __APPLE__
                glVertexAttribIPointerEXT(location, this->componentCount, this->type, this->stride, 0);
#else
                glVertexAttribIPointer(location, this->componentCount, this->type, this->stride, 0);
#endif
            } else {
                glVertexAttribPointer(location, this->componentCount, this->type, this->normalize, this->stride, 0);
            }
            glBindBuffer(GL_ARRAY_BUFFER, 0);
        }

         void disable(UInt32 location) override {
            glDisableVertexAttribArray(location);
        }

        void updateBufferData(void * data) override {
            glBindBuffer(GL_ARRAY_BUFFER, this->bufferID);
            glBufferData(GL_ARRAY_BUFFER, this->size, data, GL_DYNAMIC_DRAW);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
        }

    private:
        UInt32 size;
        UInt32 componentCount;
        GLuint bufferID;
        GLenum type;
        GLboolean normalize;
        GLsizei stride;

        void buildGPUBuffer() {
            glGenBuffers(1, &this->bufferID);
        }

        void destroyGPUBuffer() {
            glDeleteBuffers(1, &this->bufferID);
        }

    };
}
