#pragma once

#include "Material.h"
#include "../image/CubeTexture.h"

namespace Core {

  // forward declarations
  class Engine;

  class BasicCubeMaterial : public Material {
    friend class Engine;

  public:
    
    virtual Bool build() override;
    virtual GLint getShaderLocation(StandardAttributes attribute) override;
    virtual GLint getShaderLocation(StandardUniforms uniform) override;
    virtual void sendCustomUniformsToShader() override;
    void setSkyboxTexture(std::shared_ptr<CubeTexture> texture);
    
  private:
    BasicCubeMaterial(std::shared_ptr<Graphics> graphics);
    GLint positionLocation;
    GLint colorLocation;
    GLint skyboxLocation;
    std::shared_ptr<CubeTexture> skyboxTexture;
    GLint projectionMatrixLocation;
    GLint viewMatrixLocation;
  };
}
