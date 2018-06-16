#pragma once

#include <vector>
#include <memory>
#include <typeinfo>
#include <unordered_map>

#include "Object3DComponent.h"
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
    std::vector<std::shared_ptr<Object3D>>::iterator beginIterateChildren();
    std::vector<std::shared_ptr<Object3D>>::iterator endIterateChildren();
    std::vector<std::weak_ptr<Object3DComponent>>::iterator beginIterateComponents();
    std::vector<std::weak_ptr<Object3DComponent>>::iterator endIterateComponents();
    UInt32 size() const;
    void addChild(std::weak_ptr<Object3D> object);
    void removeChild(std::weak_ptr<Object3D> object);
    std::weak_ptr<Object3D> getParent();
    Bool addComponent(std::weak_ptr<Object3DComponent> component);
  
  protected:
    Object3D();
    
    Transform transform;
    std::vector<std::shared_ptr<Object3D>> children;
    std::weak_ptr<Object3D> parent;
    std::vector<std::weak_ptr<Object3DComponent>> components;
  
  };

}
