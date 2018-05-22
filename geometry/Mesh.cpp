//
// Created by Mark Kellogg on 12/13/17.
//

#include "../common/types.h"
#include "Mesh.h"

namespace MPM {

  Mesh::Mesh(UInt32 size, Bool indexed):
    size(size),  vertexPositions(nullptr), vertexColors(nullptr), vertexUVs(nullptr), indexed(indexed) {
    initAttributes();
    if (indexed) this->initIndices();
  }

  Mesh::~Mesh() {
    if (this->indexBuffer) glDeleteBuffers(1, &this->indexBuffer);
    if (this->vertexPositions) delete this->vertexPositions;
    if (this->vertexColors) delete this->vertexColors;
    if (this->vertexUVs) delete this->vertexUVs;
  }

  UInt32 Mesh::getSize() const {
    return this->size;
  }

  AttributeArray<Vector3rs>* Mesh::getVertexPositions() {
    return this->vertexPositions;
  }

  AttributeArray<ColorS>* Mesh::getVertexColors() {
    return this->vertexColors;
  }

  AttributeArray<Vector2rs>* Mesh::getVertexUVs() {
    return this->vertexUVs;
  }

  Bool Mesh::initVertexPositions(UInt32 size) {
    if (this->vertexPositions) delete this->vertexPositions;
    this->vertexPositions = new (std::nothrow) AttributeArray<Vector3rs>(size);
    ASSERT(this->vertexPositions, "Mesh::initVertexPositions() -> Unable to allocate positions array.");
    return true;
  }

  Bool Mesh::initVertexColors(UInt32 size) {
    if (this->vertexColors) delete this->vertexColors;
    this->vertexColors = new (std::nothrow) AttributeArray<ColorS>(size);
    ASSERT(this->vertexColors, "Mesh::initVertexColors() -> Unable to allocate colors array.");
    return true;
  }

  Bool Mesh::initVertexUVs(UInt32 size) {
    if (this->vertexUVs) delete this->vertexUVs;
    this->vertexUVs = new (std::nothrow) AttributeArray<Vector2rs>(size);
    ASSERT(this->vertexUVs, "Mesh::initVertexUVs() -> Unable to allocate uvs array.");
    return true;
  }

  void Mesh::initAttributes() {
    for(UInt32 i = 0; i < (UInt32)StandardAttributes::_Count; i ++) {
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

  void Mesh::initIndices() {
    glGenBuffers(1, &this->indexBuffer);
    ASSERT(this->indexBuffer, "Mesh::initIndices() -> Unable to generate index buffer.");
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->indexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  }

  void Mesh::setIndices(UInt32 * indices) {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->size * sizeof(UInt32), indices, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  }

  Bool Mesh::isIndexed() {
    return this->indexed;
  }

  GLuint Mesh::getIndexBuffer() {
    return this->indexBuffer;
  }

  void Mesh::calculateBoundingBox() {

    Vector3r min;
    Vector3r max;

    if (this->vertexPositions && this->isAttributeEnabled(StandardAttributes::Position)) {

      UInt32 index = 0;
      for(auto itr = this->vertexPositions->begin(); itr != this->vertexPositions->end(); ++itr) {
        Vector3rs& position = *itr;

        if (index == 0 || position.x < min.x) min.x = position.x;
        if (index == 0 || position.y < min.y) min.y = position.y;
        if (index == 0 || position.z < min.z) min.z = position.z;

        if (index == 0 || position.x > max.x) max.x = position.x;
        if (index == 0 || position.y > max.y) max.y = position.y;
        if (index == 0 || position.z > max.z) max.z = position.z;
        index ++;
      }

    }

    this->boundingBox.setMin(min);
    this->boundingBox.setMax(max);
  }

  const Box3& Mesh::getBoundingBox() const {
    return this->boundingBox;
  }
}
