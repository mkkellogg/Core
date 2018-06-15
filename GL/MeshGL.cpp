#include "MeshGL.h"
#include "AttributeArrayGPUStorageGL.h"
#include "../common/Exception.h"
#include "../common/types.h"

namespace Core {

    MeshGL::MeshGL(UInt32 vertexCount, Bool indexed): Mesh(vertexCount, indexed) {
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

    Bool MeshGL::initVertexPositions(UInt32 vertexCount) {
        if (this->vertexPositions) delete this->vertexPositions;

        this->vertexPositions = new (std::nothrow) AttributeArray<Vector3rs>(vertexCount);
        if (this->vertexPositions == nullptr) {
            throw AllocationException("MeshGL::initVertexPositions() -> Unable to allocate positions array.");
        } 

        AttributeArrayGPUStorageGL* gpuStorage = new (std::nothrow) AttributeArrayGPUStorageGL(this->vertexPositions->getSize(), Vector3rs::ComponentCount, GL_FLOAT, GL_FALSE, 0);
        if (gpuStorage == nullptr) {
            throw AllocationException("MeshGL::initVertexPositions() -> Unable to allocate gpu buffer.");
        } 
        std::shared_ptr<AttributeArrayGPUStorageGL> gpuStoragePtr(gpuStorage);

        this->vertexPositions->setGPUStorage(std::static_pointer_cast<AttributeArrayGPUStorage>(gpuStoragePtr));
        return true;
    }

    Bool MeshGL::initVertexColors(UInt32 vertexCount) {
        if (this->vertexColors) delete this->vertexColors;

        this->vertexColors = new (std::nothrow) AttributeArray<ColorS>(vertexCount);
        if (this->vertexColors == nullptr) {
            throw AllocationException("MeshGL::initVertexColors() -> Unable to allocate colors array.");
        }

        AttributeArrayGPUStorageGL* gpuStorage = new (std::nothrow) AttributeArrayGPUStorageGL(this->vertexColors->getSize(), ColorS::ComponentCount, GL_FLOAT, GL_FALSE, 0);
        if (gpuStorage == nullptr) {
            throw AllocationException("MeshGL::initVertexColors() -> Unable to allocate gpu buffer.");
        } 
        std::shared_ptr<AttributeArrayGPUStorageGL> gpuStoragePtr(gpuStorage);


        this->vertexColors->setGPUStorage(std::static_pointer_cast<AttributeArrayGPUStorage>(gpuStoragePtr));
        return true;
    }

    Bool MeshGL::initVertexUVs(UInt32 vertexCount) {
        if (this->vertexUVs) delete this->vertexUVs;

        this->vertexUVs = new (std::nothrow) AttributeArray<Vector2rs>(vertexCount);
        if (this->vertexUVs == nullptr) {
            throw AllocationException("MeshGL::initVertexUVs() -> Unable to allocate uvs array.");
        }

        AttributeArrayGPUStorageGL* gpuStorage = new (std::nothrow) AttributeArrayGPUStorageGL(this->vertexUVs->getSize(), Vector2rs::ComponentCount, GL_FLOAT, GL_FALSE, 0);
        if (gpuStorage == nullptr) {
            throw AllocationException("MeshGL::initVertexUVs() -> Unable to allocate gpu buffer.");
        } 
        std::shared_ptr<AttributeArrayGPUStorageGL> gpuStoragePtr(gpuStorage);
        

        this->vertexUVs->setGPUStorage(std::static_pointer_cast<AttributeArrayGPUStorage>(gpuStoragePtr));
        return true;
    }

    Bool MeshGL::initIndices() {
        if (this->indexBuffer) delete this->indexBuffer;
        this->indexBuffer = new (std::nothrow) IndexBufferGL(vertexCount);
        if (this->indexBuffer == nullptr) {
            throw AllocationException("MeshGL::initIndices() -> Unable to allocate index buffer.");
        }
        return true;
    }

    IndexBuffer* MeshGL::getIndexBuffer() {
        return this->indexBuffer;
    }

}
