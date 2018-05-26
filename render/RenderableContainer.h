#pragma once

#include "../common/types.h"
#include "../math/Matrix4x4.h"
#include "../scene/Object3D.h"
#include "../geometry/Vector3.h"

namespace Core {
  class RenderableContainer : public Object3D {

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

    RenderableContainer();
    
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
    }

    std::vector<std::shared_ptr<BaseObjectRenderer>>::const_iterator endIterateRenderers() {
      return this->renderers.getRendererList().end();
    }

  };
}