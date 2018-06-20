#pragma once

#include "../util/WeakPointer.h"
#include "Material.h"
#include "../image/Texture.h"

namespace Core {

  // forward declarations
  class Engine;

  class BasicMaterial : public Material {
    friend class Engine;
    
  public:
    virtual Bool build() override;
    virtual Int32 getShaderLocation(StandardAttributes attribute) override;
    virtual Int32 getShaderLocation(StandardUniform uniform) override;
    virtual void sendCustomUniformsToShader() override;
  
  private:
    BasicMaterial(WeakPointer<Graphics> graphics);
    GLint positionLocation;
    GLint colorLocation;
    GLint uvLocation;
    GLint projectionMatrixLocation;
    GLint viewMatrixLocation;
    GLint modelMatrixLocation;
  };
}
