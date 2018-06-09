#pragma once

#include "Material.h"
#include "../image/Texture.h"

namespace Core {

  // forward declaration
  class Engine;

  class BasicMaterial : public Material {
    friend class Engine;
    
  public:
    virtual Bool build() override;
    virtual GLint getShaderLocation(StandardAttributes attribute) override;
    virtual GLint getShaderLocation(StandardUniforms uniform) override;
    virtual void sendCustomUniformsToShader() override;
  
  private:
    BasicMaterial();
    GLint positionLocation;
    GLint colorLocation;
    GLint uvLocation;
    GLint projectionMatrixLocation;
    GLint viewMatrixLocation;
    GLint modelMatrixLocation;
  };
}
