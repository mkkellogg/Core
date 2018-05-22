#pragma once

#include "Material.h"
#include "../image/CubeTexture.h"

namespace Core {
  class BasicCubeMaterial : public Material {
    GLuint positionLocation;
    GLuint colorLocation;
    GLuint skyboxLocation;
    std::shared_ptr<CubeTexture> skyboxTexture;
    GLuint projectionMatrixLocation;
    GLuint viewMatrixLocation;
  public:
    BasicCubeMaterial();
    virtual Bool build() override;
    virtual GLuint getShaderLocation(StandardAttributes attribute) override;
    virtual GLuint getShaderLocation(StandardUniforms uniform) override;
    virtual void sendCustomUniformsToShader() override;
    void setSkyboxTexture(std::shared_ptr<CubeTexture> texture);
  };
}
