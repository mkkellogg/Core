#pragma once

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
#include "IndexBuffer.h"
#include "../material/StandardAttributes.h"
#include "Box3.h"
#include "../Graphics.h"

namespace Core {

  // forward declarations
  class Engine;

  class Mesh : public Renderable<Mesh>  {
    friend class Engine;

  public:

    virtual ~Mesh();

    virtual void init();

    UInt32 getVertexCount() const;

    AttributeArray<Vector3rs>* getVertexPositions();
    AttributeArray<ColorS>* getVertexColors();
    AttributeArray<Vector2rs>* getVertexUVs();
    std::shared_ptr<IndexBuffer> getIndexBuffer();

    Bool initVertexPositions(UInt32 vertexCount);
    Bool initVertexColors(UInt32 vertexCount);
    Bool initVertexUVs(UInt32 vertexCount);

    void enableAttribute(StandardAttributes attribute);
    void disableAttribute(StandardAttributes attribute);
    Bool isAttributeEnabled(StandardAttributes attribute);
    Bool isIndexed();

    void calculateBoundingBox();
    const Box3& getBoundingBox() const;

  protected:
    Mesh(std::shared_ptr<Graphics> graphics, UInt32 vertexCount, Bool indexed);
    void initAttributes();
    Bool initIndices();

    std::shared_ptr<Graphics> graphics;
    Bool initialized;
    Bool enabledAttributes[(UInt32)StandardAttributes::_Count];
    UInt32 vertexCount;
    Bool indexed;
    Box3 boundingBox;

    AttributeArray<Vector3rs>* vertexPositions;
    AttributeArray<ColorS>* vertexColors;
    AttributeArray<Vector2rs>* vertexUVs;
    std::shared_ptr<IndexBuffer> indexBuffer;
  };

}
