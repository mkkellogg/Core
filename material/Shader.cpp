//
// Created by Mark Kellogg on 12/15/17.
//

#include "Shader.h"
#include "../common/debug.h"
#include <stdlib.h>
#include <string.h>

namespace MPM {

  Shader::Shader() {

  }

  Shader::~Shader() {
    if (this->glProgram) {
      glDeleteProgram(this->glProgram);
      this->glProgram = 0;
    }
  }

  Shader::Shader(const std::string& vertex, const std::string& fragment): ready(false), glProgram(0) {
    this->vertexSource = vertex;
    this->fragmentSource = fragment;
  }

  Shader::Shader(const char vertex[], const char fragment[]): ready(false) {
    this->vertexSource = vertex;
    this->fragmentSource = fragment;
  }

  Bool Shader::isReady() {
    return this->ready;
  }

  Bool Shader::build() {
    GLuint program = this->createProgram(this->vertexSource, this->fragmentSource);
    return program ? true : false;
  }

  Bool Shader::checkGlError(const char *funcName) {
    static char errorString[128];
    GLint err = glGetError();
    if (err != GL_NO_ERROR) {
      sprintf(errorString, "GL error after %s(): 0x%08x\n", funcName, err);
      DebugPrintError(errorString);
      return true;
    }
    return false;
  }

  GLuint Shader::createShader(GLenum shaderType, const std::string& src) {
    static char errorString[128];
    GLuint shader = glCreateShader(shaderType);
    if (!shader) {
      Shader::checkGlError("glCreateShader");
      return 0;
    }
    const char *srcPtr = src.c_str();
    glShaderSource(shader, 1, &srcPtr, NULL);

    GLint compiled = GL_FALSE;
    glCompileShader(shader);
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
    if (!compiled) {
      GLint infoLogLen = 0;
      glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLen);
      if (infoLogLen > 0) {
        GLchar *infoLog = (GLchar *) malloc(infoLogLen);
        if (infoLog) {
          glGetShaderInfoLog(shader, infoLogLen, NULL, infoLog);
          sprintf(errorString, "Could not compile %s shader:\n%s\n",
                shaderType == GL_VERTEX_SHADER ? "vertex" : "fragment",
                infoLog);
          DebugPrintError(errorString);
          free(infoLog);
        }
      }
      glDeleteShader(shader);
      return 0;
    }
    return shader;
  }

  GLuint Shader::createProgram(const std::string& vertex, const std::string& fragment) {
    static char errorString[128];
    this->ready = false;
    GLuint vtxShader = 0;
    GLuint fragShader = 0;
    GLuint program = 0;
    GLint linked = GL_FALSE;

    vtxShader = createShader(GL_VERTEX_SHADER, vertex);
    if (!vtxShader)
      goto exit;

    fragShader = createShader(GL_FRAGMENT_SHADER, fragment);
    if (!fragShader)
      goto exit;

    program = glCreateProgram();
    if (!program) {
      checkGlError("glCreateProgram");
      goto exit;
    }
    glAttachShader(program, vtxShader);
    glAttachShader(program, fragShader);

    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &linked);
    if (!linked) {
      DebugPrintError("Could not link program");
      GLint infoLogLen = 0;
      glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLen);
      if (infoLogLen) {
        GLchar *infoLog = (GLchar *) malloc(infoLogLen);
        if (infoLog) {
          glGetProgramInfoLog(program, infoLogLen, NULL, infoLog);
          sprintf(errorString, "Could not link program:\n%s\n", infoLog);
          DebugPrintError(errorString);
          free(infoLog);
        }
      }
      glDeleteProgram(program);
      program = 0;
    }

    this->ready = true;
    this->glProgram = program;
    exit:
    if (vtxShader) glDeleteShader(vtxShader);
    if (fragShader) glDeleteShader(fragShader);
    return program;
  }

  GLuint Shader::getProgram() {
    return this->glProgram;
  }
}