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

  class Object3D {
  public:
    Object3D();
    virtual ~Object3D();

    Transform& getTransform();
    std::vector<std::shared_ptr<Object3D>>::const_iterator beginIterateChildren();
    std::vector<std::shared_ptr<Object3D>>::const_iterator endIterateChildren();
    UInt32 size() const;
    void addObject(std::weak_ptr<Object3D> object);
    void removeObject(std::weak_ptr<Object3D> object);
    std::weak_ptr<Object3D> getParent();

  private:
    std::vector<std::shared_ptr<Object3D>> children;
    std::weak_ptr<Object3D> parent;
  
  protected:
    Transform transform;
  };

}
