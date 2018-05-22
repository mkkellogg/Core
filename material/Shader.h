//
// Created by Mark Kellogg on 12/15/17.
//

#ifndef MPM_SHADER_H
#define MPM_SHADER_H

#include <string>
#include "../common/gl.h"
#include "../common/types.h"

namespace MPM {
  class Shader {

    Bool ready;
    std::string vertexSource;
    std::string fragmentSource;
    static Bool checkGlError(const char *funcName);
    GLuint createShader(GLenum shaderType, const std::string& src);
    GLuint createProgram(const std::string& vertex, const std::string& fragment);

  protected:
    GLuint glProgram;

  public:
    Shader();
    virtual ~Shader();

    Shader(const std::string& vertex, const std::string& fragment);
    Shader(const char vertex[], const char fragment[]);
    Bool build();
    Bool isReady();
    GLuint getProgram();

  };
}


#endif //MPM_SHADER_H
