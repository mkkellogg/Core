#include "MeshGL.h"
#include "../common/Exception.h"
#include "../common/types.h"

namespace Core {

    MeshGL::MeshGL(UInt32 size, Bool indexed): Mesh(size, indexed) {
        this->vertexPositions = nullptr;
        this->vertexColors = nullptr;
        this->vertexUVs = nullptr;
        this->indexBuffer = 0;
        initAttributes();
        if (indexed) this->initIndices();
    }

    MeshGL::~MeshGL() {
        if (this->indexBuffer) glDeleteBuffers(1, &this->indexBuffer);
        if (this->vertexPositions) delete this->vertexPositions;
        if (this->vertexColors) delete this->vertexColors;
        if (this->vertexUVs) delete this->vertexUVs;
    }

    AttributeArray<Vector3rs>* MeshGL::getVertexPositions() {
        return this->vertexPositions;
    }

    AttributeArray<ColorS>* MeshGL::getVertexColors() {
        return this->vertexColors;
    }

    AttributeArray<Vector2rs>* MeshGL::getVertexUVs() {
        return this->vertexUVs;
    }

    Bool MeshGL::initVertexPositions(UInt32 size) {
        if (this->vertexPositions) delete this->vertexPositions;
        this->vertexPositions = new (std::nothrow) AttributeArrayGL<Vector3rs>(size, GL_FLOAT, GL_FALSE, 0);
        if (this->vertexPositions == nullptr) {
            throw AllocationException("Mesh::initVertexPositions() -> Unable to allocate positions array.");
        } 
        return true;
    }

    Bool MeshGL::initVertexColors(UInt32 size) {
        if (this->vertexColors) delete this->vertexColors;
        this->vertexColors = new (std::nothrow) AttributeArrayGL<ColorS>(size, GL_FLOAT, GL_FALSE, 0);
        if (this->vertexColors == nullptr) {
            throw AllocationException("Mesh::initVertexColors() -> Unable to allocate colors array.");
        }
        return true;
    }

    Bool MeshGL::initVertexUVs(UInt32 size) {
        if (this->vertexUVs) delete this->vertexUVs;
        this->vertexUVs = new (std::nothrow) AttributeArrayGL<Vector2rs>(size, GL_FLOAT, GL_FALSE, 0);
        if (this->vertexUVs == nullptr) {
            throw AllocationException("Mesh::initVertexUVs() -> Unable to allocate uvs array.");
        }
        return true;
    }

    void MeshGL::initIndices() {
        glGenBuffers(1, &this->indexBuffer);
        if (!this->indexBuffer) {
            throw AllocationException("Mesh::initIndices() -> Unable to generate index buffer.");
        }
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->indexBuffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    void MeshGL::setIndices(UInt32* indices) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->indexBuffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->size * sizeof(UInt32), indices, GL_DYNAMIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    GLuint MeshGL::getIndexBuffer() {
        return this->indexBuffer;
    }

}
