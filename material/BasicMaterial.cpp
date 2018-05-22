//
// Created by Mark Kellogg on 12/15/17.
//

#include "BasicMaterial.h"
#include "StandardAttributes.h"
#include "StandardUniforms.h"

static const char vertexShader[] =
    "#version 100\n"
        "attribute vec4 pos;\n"
        "attribute vec4 color;\n"
        "attribute vec2 uv;\n"
        "uniform mat4 projection;\n"
        "uniform mat4 viewMatrix;\n"
        "varying vec4 vColor;\n"
        "void main() {\n"
        "    gl_Position = projection * viewMatrix * pos;\n"
        "    vColor = color;\n"
        "}\n";

static const char fragmentShader[] =
    "#version 100\n"
        "precision mediump float;\n"
        "varying vec4 vColor;\n"
        "void main() {\n"
        "    gl_FragColor = vColor;\n"
        "}\n";

namespace MPM {

  BasicMaterial::BasicMaterial() {

  }

  Bool BasicMaterial::build() {
    std::string vertexSrc = vertexShader;
    std::string fragmentSrc = fragmentShader;
    Bool ready = this->buildFromSource(vertexSrc, fragmentSrc);
    if (!ready) {
      return false;
    }
    this->positionLocation = glGetAttribLocation(this->shader->getProgram(), "pos");
    this->colorLocation = glGetAttribLocation(this->shader->getProgram(), "color");
    this->projectionMatrixLocation = glGetUniformLocation(this->shader->getProgram(), "projection");
    this->viewMatrixLocation = glGetUniformLocation(this->shader->getProgram(), "viewMatrix");
    return true;
  }

  GLuint BasicMaterial::getShaderLocation(StandardAttributes attribute) {
    switch(attribute) {
      case StandardAttributes::Position:
        return this->positionLocation;
      case StandardAttributes::Color:
        return this->colorLocation;
      case StandardAttributes::UV:
        return this->uvLocation;
      default:
        return -1;
    }
  }

  GLuint BasicMaterial::getShaderLocation(StandardUniforms uniform) {
    switch(uniform) {
      case StandardUniforms::ProjectionMatrix:
        return this->projectionMatrixLocation;
      case StandardUniforms::ViewMatrix:
        return this->viewMatrixLocation;
      default:
        return -1;
    }
  }

  void BasicMaterial::sendCustomUniformsToShader() {

  }
}