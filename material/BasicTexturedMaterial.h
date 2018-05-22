//
// Created by Mark Kellogg on 12/15/17.
//

#ifndef MPM_BASIC_TEXTURED_MATERIAL_H
#define MPM_BASIC_TEXTURED_MATERIAL_H

#include "Material.h"
#include "../image/Texture.h"

namespace Core {
  class BasicTexturedMaterial : public Material {
    GLuint positionLocation;
    GLuint colorLocation;
    GLuint textureLocation;
    GLuint uvLocation;
    std::shared_ptr<Texture> texture;
    GLuint projectionMatrixLocation;
    GLuint viewMatrixLocation;
  public:
    BasicTexturedMaterial();
    virtual Bool build() override;
    virtual GLuint getShaderLocation(StandardAttributes attribute) override;
    virtual GLuint getShaderLocation(StandardUniforms uniform) override;
    virtual void sendCustomUniformsToShader() override;
    void setTexture(std::shared_ptr<Texture> texture);
  };
}


#endif //MPM_BASIC_TEXTURED_MATERIAL_H
