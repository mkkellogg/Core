//
// Created by Mark Kellogg on 12/15/17.
//

#ifndef MPM_BASIC_MATERIAL_H
#define MPM_BASIC_MATERIAL_H

#include "Material.h"
#include "../image/Texture.h"

namespace MPM {
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


#endif //MPM_BASIC_MATERIAL_H
