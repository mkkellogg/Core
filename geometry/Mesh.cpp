#include "Mesh.h"
#include "../common/Exception.h"
#include "../common/types.h"

namespace Core {

    Mesh::Mesh(UInt32 size, Bool indexed) : size(size), indexed(indexed) {
        initAttributes();
        if (indexed) this->initIndices();
    }

    Mesh::~Mesh() {

    }

    UInt32 Mesh::getSize() const {
        return this->size;
    }

    void Mesh::initAttributes() {
        for (UInt32 i = 0; i < (UInt32)StandardAttributes::_Count; i++) {
            this->disableAttribute((StandardAttributes)i);
        }
    }

    void Mesh::enableAttribute(StandardAttributes attribute) {
        this->enabledAttributes[(UInt32)attribute] = true;
    }

    void Mesh::disableAttribute(StandardAttributes attribute) {
        this->enabledAttributes[(UInt32)attribute] = false;
    }

    Bool Mesh::isAttributeEnabled(StandardAttributes attribute) {
        return this->enabledAttributes[(UInt32)attribute];
    }

    Bool Mesh::isIndexed() {
        return this->indexed;
    }

    void Mesh::calculateBoundingBox() {
        Vector3r min;
        Vector3r max;

        AttributeArray<Vector3rs>* vertexPositions = this->getVertexPositions();

        if (vertexPositions && this->isAttributeEnabled(StandardAttributes::Position)) {
            UInt32 index = 0;
            for (auto itr = vertexPositions->begin(); itr != vertexPositions->end(); ++itr) {
                Vector3rs& position = *itr;

                if (index == 0 || position.x < min.x) min.x = position.x;
                if (index == 0 || position.y < min.y) min.y = position.y;
                if (index == 0 || position.z < min.z) min.z = position.z;

                if (index == 0 || position.x > max.x) max.x = position.x;
                if (index == 0 || position.y > max.y) max.y = position.y;
                if (index == 0 || position.z > max.z) max.z = position.z;
                index++;
            }
        }

        this->boundingBox.setMin(min);
        this->boundingBox.setMax(max);
    }

    const Box3& Mesh::getBoundingBox() const {
        return this->boundingBox;
    }
}
