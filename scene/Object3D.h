//
// Created by Mark Kellogg on 12/14/17.
//

#ifndef MPM_OBJECT3D_H
#define MPM_OBJECT3D_H

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

namespace MPM {

  // forward declarations
  class Mesh;
  class MeshRenderer;

  class Object3D {
    Transform localTransform;
    Transform worldTransform;
    std::vector<std::shared_ptr<Object3D>> children;
    std::unordered_map<UInt32, std::vector<std::shared_ptr<BaseRenderable>>> mappedRenderables;
    ObjectRenderers renderers;

    template <typename T>
    UInt32 getTypeCode() {
      return (UInt32)typeid(T).hash_code();
    }

    template <typename T>
    Bool checkRenderableTypeInitialized() {
      UInt32 typeIDOut;
      return this->checkRenderableTypeInitialized<T>(typeIDOut);
    }

    template <typename T>
    Bool checkRenderableTypeInitialized(UInt32& typeIDOut) {
      typeIDOut = this->getTypeCode<T>();
      auto found = mappedRenderables.find (typeIDOut);
      return !(found == mappedRenderables.end());
    }

    template <typename T>
    UInt32 ensureRenderableTypeInitialized() {
      UInt32 typeID;
      Bool typeExists = this->checkRenderableTypeInitialized<T>(typeID);
      if (!typeExists) {
        mappedRenderables[typeID] = std::vector<std::shared_ptr<BaseRenderable>>();
      }
      return typeID;
    }

  public:
    Object3D();

    template <typename T>
    void addRenderable(std::shared_ptr<Renderable<T>> renderable) {
      UInt32 typeID = this->ensureRenderableTypeInitialized<T>();
      std::vector<std::shared_ptr<BaseRenderable>>& renderables = this->mappedRenderables[typeID];
      renderables.push_back(renderable);
    }

    template <typename T>
    const std::vector<std::shared_ptr<T>>& getRenderables() {
      UInt32 typeID = this->ensureRenderableTypeInitialized<T>();
      return (std::vector<std::shared_ptr<T>>&)this->mappedRenderables[typeID];
    }

    template <typename T>
    void setCustomRenderer(std::shared_ptr<ObjectRenderer<T>> renderer) {
      this->renderers.setRenderer(renderer);
    }

    template <typename T>
    std::shared_ptr<ObjectRenderer<T>> getCustomRenderer() {
      return this->renderers.getRenderer<T>();
    }

    std::vector<std::shared_ptr<BaseObjectRenderer>>::const_iterator beginIterateRenderers() {
      return this->renderers.getRendererList().begin();
    };

    std::vector<std::shared_ptr<BaseObjectRenderer>>::const_iterator endIterateRenderers() {
      return this->renderers.getRendererList().end();
    };

    Transform& getLocalTransform();
    Transform& getWorldTransform();
    std::vector<std::shared_ptr<Object3D>>::const_iterator beginIterateChildren();
    std::vector<std::shared_ptr<Object3D>>::const_iterator endIterateChildren();
    UInt32 size() const;
    void addObject(std::shared_ptr<Object3D> object);
  };

}

#endif //MPM_OBJECT3D_H
