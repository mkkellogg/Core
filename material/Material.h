//
// Created by Mark Kellogg on 12/15/17.
//

#ifndef MPM_MATERIAL_H
#define MPM_MATERIAL_H

#include <memory>
#include "../common/gl.h"
#include "Shader.h"
#include "StandardAttributes.h"
#include "StandardUniforms.h"

namespace MPM {
  class Material {
  protected:
    std::shared_ptr<Shader> shader;
    Bool ready;
    Bool buildFromSource(const std::string& vertexSource, const std::string& fragmentSource);
    void setShader(std::shared_ptr<Shader> shader);
  public:
    Material();
    Material(std::shared_ptr<Shader> shader);
    std::shared_ptr<Shader> getShader();
    virtual Bool build() = 0;
    virtual GLuint getShaderLocation(StandardAttributes attribute) = 0;
    virtual GLuint getShaderLocation(StandardUniforms uniform) = 0;
    virtual void sendCustomUniformsToShader() = 0;
  };
}


#endif //MPM_MATERIAL_H
