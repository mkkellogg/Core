#pragma once

#include "Material.h"
#include "../image/Texture.h"

namespace Core {
  class BasicMaterial : public Material {
    GLuint positionLocation;
    GLuint colorLocation;
    GLuint uvLocation;
    GLuint projectionMatrixLocation;
    GLuint viewMatrixLocation;
  public:
    BasicMaterial();
    virtual Bool build() override;
    virtual GLuint getShaderLocation(StandardAttributes attribute) override;
    virtual GLuint getShaderLocation(StandardUniforms uniform) override;
    virtual void sendCustomUniformsToShader() override;
  };
}
