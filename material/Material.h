#pragma once

#include <memory>
#include "../common/gl.h"
#include "Shader.h"
#include "StandardAttributes.h"
#include "StandardUniforms.h"

namespace Core {
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
    virtual GLint getShaderLocation(StandardAttributes attribute) = 0;
    virtual GLint getShaderLocation(StandardUniforms uniform) = 0;
    virtual void sendCustomUniformsToShader() = 0;
  };
}
