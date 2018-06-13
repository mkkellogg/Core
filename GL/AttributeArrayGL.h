#pragma once

#include <string.h>
#include <new>

#include "../geometry/AttributeArray.h"

namespace Core {

    template <typename T>
    class AttributeArrayGL final: public AttributeArray<T> {
    public:
        AttributeArrayGL(UInt32 attributeCount) : AttributeArray<T>(attributeCount) {
            buildGPUBuffer();
        }

        ~AttributeArrayGL() {
            destroyGPUBuffer();
        }

        Int32 getBufferID() const override {
            return this->bufferID;
        }

    private:
        GLuint bufferID;

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
