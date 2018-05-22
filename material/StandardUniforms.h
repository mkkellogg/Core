//
// Created by Mark Kellogg on 12/15/17.
//

#ifndef MPM_UNIFORMS_H
#define MPM_UNIFORMS_H

namespace Core {

  enum class StandardUniforms {
    ModelMatrix = 0,
    ViewMatrix = 1,
    ModelViewMatrix = 2,
    ProjectionMatrix = 3,
    _Count = 4 // Must always be last in the list
  };

}

#endif //MPM_UNIFORMS_H
