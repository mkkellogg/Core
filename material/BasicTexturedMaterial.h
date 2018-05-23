#pragma once

#include "Material.h"
#include "../image/Texture.h"

namespace Core {
  class BasicTexturedMaterial : public Material {
    GLint positionLocation;
    GLint colorLocation;
    GLint textureLocation;
    GLint uvLocation;
    std::shared_ptr<Texture> texture;
    GLint projectionMatrixLocation;
    GLint viewMatrixLocation;
  public:
    BasicTexturedMaterial();
    virtual Bool build() override;
    virtual GLint getShaderLocation(StandardAttributes attribute) override;
    virtual GLint getShaderLocation(StandardUniforms uniform) override;
    virtual void sendCustomUniformsToShader() override;
    void setTexture(std::shared_ptr<Texture> texture);
  };
}
