#include "MeshGL.h"
#include "../common/Exception.h"
#include "../common/types.h"

namespace Core {

    MeshGL::MeshGL(UInt32 size, Bool indexed): Mesh(size, indexed) {
        this->vertexPositions = nullptr;
        this->vertexColors = nullptr;
        this->vertexUVs = nullptr;
        this->indexBuffer = nullptr;
        initAttributes();
    }

    MeshGL::~MeshGL() {
        if (this->indexBuffer) {
            delete this->indexBuffer;
            this->indexBuffer = nullptr;
        }
        if (this->vertexPositions) {
            delete this->vertexPositions;
            this->vertexPositions = nullptr;
        }
        if (this->vertexColors) {
            delete this->vertexColors;
            this->vertexColors = nullptr;
        }
        if (this->vertexUVs) {
            delete this->vertexUVs;
            this->vertexUVs = nullptr;
        }
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
            throw AllocationException("MeshGL::initVertexPositions() -> Unable to allocate positions array.");
        } 
        return true;
    }

    Bool MeshGL::initVertexColors(UInt32 size) {
        if (this->vertexColors) delete this->vertexColors;
        this->vertexColors = new (std::nothrow) AttributeArrayGL<ColorS>(size, GL_FLOAT, GL_FALSE, 0);
        if (this->vertexColors == nullptr) {
            throw AllocationException("MeshGL::initVertexColors() -> Unable to allocate colors array.");
        }
        return true;
    }

    Bool MeshGL::initVertexUVs(UInt32 size) {
        if (this->vertexUVs) delete this->vertexUVs;
        this->vertexUVs = new (std::nothrow) AttributeArrayGL<Vector2rs>(size, GL_FLOAT, GL_FALSE, 0);
        if (this->vertexUVs == nullptr) {
            throw AllocationException("MeshGL::initVertexUVs() -> Unable to allocate uvs array.");
        }
        return true;
    }

    Bool MeshGL::initIndices() {
        if (this->indexBuffer) delete this->indexBuffer;
        this->indexBuffer = new (std::nothrow) IndexBufferGL(size);
        if (this->indexBuffer == nullptr) {
            throw AllocationException("MeshGL::initIndices() -> Unable to allocate index buffer.");
        }
        return true;
    }

    IndexBuffer* MeshGL::getIndexBuffer() {
        return this->indexBuffer;
    }

}
