#include "BasicMaterial.h"
#include "StandardAttributes.h"
#include "StandardUniforms.h"
#include "../util/WeakPointer.h"

static const char vertexShader[] =
    "#version 100\n"
        "attribute vec4 pos;\n"
        "attribute vec4 color;\n"
        "attribute vec2 uv;\n"
        "uniform mat4 projection;\n"
        "uniform mat4 viewMatrix;\n"
        "uniform mat4 modelMatrix;\n"
        "varying vec4 vColor;\n"
        "void main() {\n"
        "    gl_Position = projection * viewMatrix * modelMatrix * pos;\n"
        "    vColor = color;\n"
        "}\n";

static const char fragmentShader[] =
    "#version 100\n"
        "precision mediump float;\n"
        "varying vec4 vColor;\n"
        "void main() {\n"
        "    gl_FragColor = vColor;\n"
        "}\n";

namespace Core {

  BasicMaterial::BasicMaterial(WeakPointer<Graphics> graphics): Material(graphics) {

  }

  Bool BasicMaterial::build() {
    std::string vertexSrc = vertexShader;
    std::string fragmentSrc = fragmentShader;
    Bool ready = this->buildFromSource(vertexSrc, fragmentSrc);
    if (!ready) {
      return false;
    }

    this->positionLocation = this->shader->getAttributeLocation("pos");
    this->colorLocation = this->shader->getAttributeLocation("color");
    this->projectionMatrixLocation = this->shader->getUniformLocation("projection");
    this->viewMatrixLocation = this->shader->getUniformLocation("viewMatrix");
    this->modelMatrixLocation = this->shader->getUniformLocation("modelMatrix");
    return true;
  }

  Int32 BasicMaterial::getShaderLocation(StandardAttributes attribute) {
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

  Int32 BasicMaterial::getShaderLocation(StandardUniforms uniform) {
    switch(uniform) {
      case StandardUniforms::ProjectionMatrix:
        return this->projectionMatrixLocation;
      case StandardUniforms::ViewMatrix:
        return this->viewMatrixLocation;
      case StandardUniforms::ModelMatrix:
        return this->modelMatrixLocation;
      default:
        return -1;
    }
  }

  void BasicMaterial::sendCustomUniformsToShader() {

  }
}