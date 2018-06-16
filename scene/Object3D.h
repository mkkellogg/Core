#pragma once

#include <vector>
#include <memory>
#include <typeinfo>
#include <unordered_map>

#include "../common/assert.h"
#include "Transform.h"
#include "../render/BaseRenderable.h"
#include "../render/Renderable.h"
#include "../render/ObjectRenderer.h"
#include "../render/ObjectRenderers.h"

namespace Core {

  // forward declarations
  class Mesh;
  class MeshRenderer;
  class Engine;

  class Object3D {
    friend class Engine;

  public:
    virtual ~Object3D();

    Transform& getTransform();
    std::vector<std::shared_ptr<Object3D>>::const_iterator beginIterateChildren();
    std::vector<std::shared_ptr<Object3D>>::const_iterator endIterateChildren();
    UInt32 size() const;
    void addChild(std::weak_ptr<Object3D> object);
    void removeChild(std::weak_ptr<Object3D> object);
    std::weak_ptr<Object3D> getParent();
  
  protected:
    Object3D();
    
    Transform transform;
    std::vector<std::shared_ptr<Object3D>> children;
    std::weak_ptr<Object3D> parent;
  
  };

}
