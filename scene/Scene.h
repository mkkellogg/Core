#pragma once

#include <vector>
#include <memory>
#include <type_traits>

#include "../common/types.h"
#include "Object3D.h"

namespace Core {

  //forward declarations
  class Engine;

  class Scene {
    friend class Engine;

  public:
    std::weak_ptr<Object3D> getRoot();

    template <typename T>
    std::weak_ptr<typename std::enable_if<std::is_base_of<Object3D, T>::value, T>::type> createObject3D() {
      std::shared_ptr<T> objPtr = std::shared_ptr<T>(new T());
      this->objects.push_back(objPtr);
      return objPtr;
    }

  private:
    Scene();

    std::shared_ptr<Object3D> root;
    std::vector<std::shared_ptr<Object3D>> objects;
  };

}
