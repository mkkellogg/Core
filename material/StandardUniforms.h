#pragma once

namespace Core {

  enum class StandardUniforms {
    ModelMatrix = 0,
    ViewMatrix = 1,
    ModelViewMatrix = 2,
    ProjectionMatrix = 3,
    _Count = 4 // Must always be last in the list
  };

}
