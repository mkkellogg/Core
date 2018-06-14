#pragma once

#include <string.h>
#include <new>

#include "../geometry/AttributeArray.h"
#include "../common/gl.h"

namespace Core {

    template <typename T>
    class AttributeArrayGL final: public AttributeArray<T> {
    public:
        AttributeArrayGL(UInt32 attributeCount, GLenum type, GLboolean normalize, GLsizei stride): 
            AttributeArray<T>(attributeCount), type(type), normalize(normalize), stride(stride) {
            buildGPUBuffer();
        }

        ~AttributeArrayGL() {
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

        void sendToShader(UInt32 location) override {
            glBindBuffer(GL_ARRAY_BUFFER, this->bufferID);
            glVertexAttribPointer(location, this->componentCount, this->type, this->normalize, this->stride, 0);
            glEnableVertexAttribArray(location);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
        }

    private:
        GLuint bufferID;
        GLenum type;
        GLboolean normalize;
        GLsizei stride;

        void buildGPUBuffer() {
            glGenBuffers(1, &this->bufferID);
            this->updateBufferData();
        }

        void destroyGPUBuffer() {
            glDeleteBuffers(1, &this->bufferID);
        }

        void updateBufferData() override {
            glBindBuffer(GL_ARRAY_BUFFER, this->bufferID);
            glBufferData(GL_ARRAY_BUFFER, this->getSize(), this->storage, GL_DYNAMIC_DRAW);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
        }
    };
}
