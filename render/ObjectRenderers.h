//
// Created by Mark Kellogg on 1/17/18.
//

#ifndef MPM_OBJECT_RENDERS_H
#define MPM_OBJECT_RENDERS_H

#include <memory>
#include <unordered_map>
#include <vector>
#include "../common/types.h"
#include "BaseObjectRenderer.h"
#include "ObjectRenderer.h"

namespace MPM {

  class ObjectRenderers {

    UInt32 nextIndex = 0;
    std::unordered_map<UInt32, UInt32> rendererIndexMap;
    std::vector<std::shared_ptr<BaseObjectRenderer>> renderers;

    template <typename T>
    UInt32 getTypeCode() {
      return (UInt32)typeid(T).hash_code();
    }

    template <typename T>
    Bool isRendererSet() {
      return this->rendererIndexMap.find(this->getTypeCode<T>()) == this->rendererIndexMap.end();
    }

    template <typename T>
    Bool getRendererIndex() {
      UInt32 typeID = this->getTypeCode<T>();
      UInt32 rendererIndex;
      if (this->rendererIndexMap.find(this->getTypeCode<T>()) == this->rendererIndexMap.end()) {
        this->rendererIndexMap[typeID] = this->renderers.size();
      }
      return this->rendererIndexMap[typeID];
    }

  public:

    template <typename T>
    void setRenderer(std::shared_ptr<ObjectRenderer<T>> renderer) {
      UInt32 typeID = this->getTypeCode<T>();
      UInt32 rendererIndex;
      if (this->rendererIndexMap.find(this->getTypeCode<T>()) == this->rendererIndexMap.end()) {
        this->rendererIndexMap[typeID] = this->renderers.size();
        this->renderers.push_back(renderer);
      }
      else {
        this->renderers[this->rendererIndexMap[typeID]] = renderer;
      }
    }

    template <typename T>
    std::shared_ptr<ObjectRenderer<T>> getRenderer() {
      UInt32 typeID = this->getTypeCode<T>();
      if (this->isRendererSet<T>()) {
        UInt32 rendererIndex = this->rendererIndexMap[typeID];
        return std::dynamic_pointer_cast<ObjectRenderer<T>>(this->renderers[rendererIndex]);
      }
      return nullptr;
    }

    const std::vector<std::shared_ptr<BaseObjectRenderer>>& getRendererList() const {
      return this->renderers;
    }

  };
}

#endif //MPM_OBJECT_RENDERS_H
