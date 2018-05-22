//
// Created by Mark Kellogg on 12/15/17.
//

#include "BasicTexturedMaterial.h"
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
        "varying vec2 vUV;\n"
        "void main() {\n"
        "    gl_Position = projection * viewMatrix * pos;\n"
        "    vUV = uv;\n"
        "    vColor = color;\n"
        "}\n";

static const char fragmentShader[] =
    "#version 100\n"
        "precision mediump float;\n"
        "uniform sampler2D textureA;\n"
        "varying vec4 vColor;\n"
        "varying vec2 vUV;\n"
        "void main() {\n"
        "    vec4 textureColor = texture2D(textureA, vUV);\n"
        "    gl_FragColor = textureColor;\n"
        "}\n";

namespace MPM {

  BasicTexturedMaterial::BasicTexturedMaterial() {

  }

  Bool BasicTexturedMaterial::build() {
    std::string vertexSrc = vertexShader;
    std::string fragmentSrc = fragmentShader;
    Bool ready = this->buildFromSource(vertexSrc, fragmentSrc);
    if (!ready) {
      return false;
    }
    this->positionLocation = glGetAttribLocation(this->shader->getProgram(), "pos");
    this->colorLocation = glGetAttribLocation(this->shader->getProgram(), "color");
    this->textureLocation = glGetAttribLocation(this->shader->getProgram(), "textureA");
    this->projectionMatrixLocation = glGetUniformLocation(this->shader->getProgram(), "projection");
    this->viewMatrixLocation = glGetUniformLocation(this->shader->getProgram(), "viewMatrix");
    this->uvLocation = glGetAttribLocation(this->shader->getProgram(), "uv");
    return true;
  }

  GLuint BasicTexturedMaterial::getShaderLocation(StandardAttributes attribute) {
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

  GLuint BasicTexturedMaterial::getShaderLocation(StandardUniforms uniform) {
    switch(uniform) {
      case StandardUniforms::ProjectionMatrix:
        return this->projectionMatrixLocation;
      case StandardUniforms::ViewMatrix:
        return this->viewMatrixLocation;
      default:
        return -1;
    }
  }

  void BasicTexturedMaterial::setTexture(std::shared_ptr<Texture> texture) {
    this->texture = texture;
  }

  void BasicTexturedMaterial::sendCustomUniformsToShader() {
    if (this->texture) {
      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D, this->texture->getTextureID());
      glUniform1i(textureLocation, 0);
    }
  }
}