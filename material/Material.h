#pragma once

#include <memory>
#include "../Graphics.h"
#include "Shader.h"
#include "StandardAttributes.h"
#include "StandardUniforms.h"

namespace Core {
  class Material {
  public:
    Material(std::shared_ptr<Graphics> graphics);
    Material(std::shared_ptr<Graphics> graphics, std::weak_ptr<Shader> shader);
    std::weak_ptr<Shader> getShader();
    virtual Bool build() = 0;

    virtual Int32 getShaderLocation(StandardAttributes attribute) = 0;
    virtual Int32 getShaderLocation(StandardUniforms uniform) = 0;
    virtual void sendCustomUniformsToShader() = 0;

  protected:
    std::shared_ptr<Graphics> graphics;
    std::weak_ptr<Shader> shader;
    Bool ready;
    Bool buildFromSource(const std::string& vertexSource, const std::string& fragmentSource);
    void setShader(std::weak_ptr<Shader> shader);
  };
}
