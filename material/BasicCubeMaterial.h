#pragma once

#include "Material.h"
#include "../image/CubeTexture.h"

namespace Core {
  class BasicCubeMaterial : public Material {
    GLint positionLocation;
    GLint colorLocation;
    GLint skyboxLocation;
    std::shared_ptr<CubeTexture> skyboxTexture;
    GLint projectionMatrixLocation;
    GLint viewMatrixLocation;
  public:
    BasicCubeMaterial();
    virtual Bool build() override;
    virtual GLint getShaderLocation(StandardAttributes attribute) override;
    virtual GLint getShaderLocation(StandardUniforms uniform) override;
    virtual void sendCustomUniformsToShader() override;
    void setSkyboxTexture(std::shared_ptr<CubeTexture> texture);
  };
}
