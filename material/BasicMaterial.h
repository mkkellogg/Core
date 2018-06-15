#pragma once

#include "Material.h"
#include "../image/Texture.h"

namespace Core {

  // forward declarations
  class Engine;

  class BasicMaterial : public Material {
    friend class Engine;
    
  public:
    virtual Bool build() override;
    virtual GLint getShaderLocation(StandardAttributes attribute) override;
    virtual GLint getShaderLocation(StandardUniforms uniform) override;
    virtual void sendCustomUniformsToShader() override;
  
  private:
    BasicMaterial(std::shared_ptr<Graphics> graphics);
    GLint positionLocation;
    GLint colorLocation;
    GLint uvLocation;
    GLint projectionMatrixLocation;
    GLint viewMatrixLocation;
    GLint modelMatrixLocation;
  };
}
