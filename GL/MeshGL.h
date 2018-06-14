#pragma once

#include <new>
#include <unordered_map>

#include "AttributeArrayGL.h"
#include "IndexBufferGL.h"
#include "../common/gl.h"
#include "../geometry/Mesh.h"
#include "../geometry/Vector3.h"
#include "../geometry/Vector2.h"
#include "../geometry/Box3.h"
#include "../common/assert.h"
#include "../common/types.h"
#include "../render/Renderable.h"
#include "../scene/Object3D.h"
#include "../color/Color.h"
#include "../material/StandardAttributes.h"

namespace Core {

  // forward declarations
  class GraphicsGL;

  class MeshGL final: public Mesh  {
    friend class GraphicsGL;

  public:

    virtual ~MeshGL();

    AttributeArray<Vector3rs>* getVertexPositions() override;
    AttributeArray<ColorS>* getVertexColors() override;
    AttributeArray<Vector2rs>* getVertexUVs() override;
    IndexBuffer* getIndexBuffer() override;

    Bool initVertexPositions(UInt32 size) override;
    Bool initVertexColors(UInt32 size) override;
    Bool initVertexUVs(UInt32 size) override;

  protected:
    MeshGL(UInt32 size, Bool indexed);  
    Bool initIndices() override;  

    AttributeArrayGL<Vector3rs>* vertexPositions;
    AttributeArrayGL<ColorS>* vertexColors;
    AttributeArrayGL<Vector2rs>* vertexUVs;
    IndexBufferGL* indexBuffer;

  };

}
