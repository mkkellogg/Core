//
// Created by Mark Kellogg on 12/13/17.
//

#ifndef MPM_MESH_H
#define MPM_MESH_H

#include <new>
#include <unordered_map>
#include "../common/assert.h"
#include "../common/types.h"
#include "../render/Renderable.h"
#include "../scene/Object3D.h"
#include "Vector3.h"
#include "Vector2.h"
#include "../color/Color.h"
#include "AttributeArray.h"
#include "../material/StandardAttributes.h"
#include "Box3.h"

namespace MPM {

  class Mesh : public Renderable<Mesh>  {

    UInt32 size;
    Bool indexed;

    Bool enabledAttributes[(UInt32)StandardAttributes::_Count];
    AttributeArray<Vector3rs>* vertexPositions;
    AttributeArray<ColorS>* vertexColors;
    AttributeArray<Vector2rs>* vertexUVs;
    GLuint indexBuffer;

    Box3 boundingBox;

    void initAttributes();
    void initIndices();

  public:

    Mesh(UInt32 size, Bool indexed);
    virtual ~Mesh();

    UInt32 getSize() const;

    AttributeArray<Vector3rs>* getVertexPositions();
    AttributeArray<ColorS>* getVertexColors();
    AttributeArray<Vector2rs>* getVertexUVs();

    Bool initVertexPositions(UInt32 size);
    Bool initVertexColors(UInt32 size);
    Bool initVertexUVs(UInt32 size);

    void enableAttribute(StandardAttributes attribute);
    void disableAttribute(StandardAttributes attribute);
    Bool isAttributeEnabled(StandardAttributes attribute);

    void setIndices(UInt32 * indices);
    Bool isIndexed();
    GLuint getIndexBuffer();

    void calculateBoundingBox();
    const Box3& getBoundingBox() const;

  };

}

#endif //MPM_MESH_H
