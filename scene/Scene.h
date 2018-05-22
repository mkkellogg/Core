//
// Created by Mark Kellogg on 12/14/17.
//

#ifndef MPM_SCENE_H
#define MPM_SCENE_H

#include <vector>
#include <memory>

#include "../common/types.h"
#include "Object3D.h"

namespace MPM {

  class Scene {

    std::shared_ptr<Object3D> root;

  public:
    Scene();

    std::shared_ptr<Object3D> getRoot();

  };

}


#endif //MPM_SCENE_H
