#include "IndexBufferGL.h"
#include "../common/Exception.h"

namespace Core {

    IndexBufferGL::IndexBufferGL(UInt32 size): IndexBuffer(size), bufferID(0) {

    }

    IndexBufferGL::~IndexBufferGL() {
        
    }

    Int32 IndexBufferGL::getBufferID() const {
        return this->bufferID;
    }

    void IndexBufferGL::destroy() {
        if (this->bufferID > 0) {
            glDeleteBuffers(1, &this->bufferID);
            this->bufferID = 0;
        }
    }
    
    void IndexBufferGL::initIndices() {
        this->destroy();
        glGenBuffers(1, &this->bufferID);
        if (!this->bufferID) {
            throw AllocationException("IndexBufferGL::initIndices() -> Unable to generate index buffer.");
        }
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->bufferID);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    void IndexBufferGL::setIndices(UInt32* indices) {
        IndexBuffer::setIndices(indices);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->bufferID);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->size * sizeof(UInt32), indices, GL_DYNAMIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

}
