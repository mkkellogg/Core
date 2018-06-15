#pragma once

#include <string>

#include "../material/Shader.h"
#include "../common/gl.h"
#include "../common/types.h"

namespace Core {

  // forward declarations
  class GraphicsGL;

  class ShaderGL final: public Shader {
    friend class GraphicsGL;

  public:
    ~ShaderGL();

    Bool isReady() const;
    Bool build() override;
    UInt32 getProgram() const override;
    Int32 getUniformLocation(const std::string& var) const override;
    Int32 getAttributeLocation(const std::string& var) const override;
    Int32 getUniformLocation(const char var[]) const override;
    Int32 getAttributeLocation(const char var[]) const override;

    void setTexture2D(UInt32 slot, UInt32 textureID) override;
    void setTextureCube(UInt32 slot, UInt32 textureID) override;
    void setUniform1i(UInt32 location, Int32 val) override;
    void setUniformMatrix4(UInt32 location, const Real * data) override;
    void setUniformMatrix4(UInt32 location, const Matrix4x4& data) override;

  protected:
    ShaderGL();
    ShaderGL(const std::string& vertex, const std::string& fragment);
    ShaderGL(const char vertex[], const char fragment[]);

    static Bool checkGlError(const char *funcName);
    GLenum convertShaderType(ShaderType shaderType);
    UInt32 createShader(ShaderType shaderType, const std::string& src) override;
    UInt32 createProgram(const std::string& vertex, const std::string& fragment) override;

    GLuint glProgram;
    
  };
}
