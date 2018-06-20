#include "BasicCubeMaterial.h"
#include "StandardAttributes.h"
#include "StandardUniforms.h"
#include "../util/WeakPointer.h"

static const char vertexShader[] =
    "#version 100\n"
        "attribute vec4 pos;\n"
        "attribute vec4 color;\n"
        "uniform mat4 projection;\n"
        "uniform mat4 viewMatrix;\n"
        "mat4 scale = mat4(1.0, 0.0, 0.0, 0.0,\n"
        "                  0.0, 1.0, 0.0, 0.0,\n"
        "                  0.0, 0.0, 1.0, -20.0,\n"
        "                  0.0, 0.0, 0.0, 1.0);\n"
        "varying vec4 vColor;\n"
        "varying vec3 vUV;\n"
        "void main() {\n"
        "    gl_Position = projection * viewMatrix * pos;\n"
        "    vUV = normalize(pos.xyz);\n"
        "    vColor = color;\n"
        "}\n";

static const char fragmentShader[] =
    "#version 100\n"
        "precision mediump float;\n"
        "uniform samplerCube skybox;\n"
        "varying vec4 vColor;\n"
        "varying vec3 vUV;\n"
        "void main() {\n"
        "    vec4 textureColor = textureCube(skybox, vUV);\n"
        "    gl_FragColor = textureColor;\n"
        "}\n";

namespace Core {

  BasicCubeMaterial::BasicCubeMaterial(WeakPointer<Graphics> graphics): Material(graphics) {

  }

  Bool BasicCubeMaterial::build() {
    std::string vertexSrc = vertexShader;
    std::string fragmentSrc = fragmentShader;
    Bool ready = this->buildFromSource(vertexSrc, fragmentSrc);
    if (!ready) {
      return false;
    }

    this->positionLocation = this->shader->getAttributeLocation("pos");
    this->colorLocation = this->shader->getAttributeLocation("color");
    this->skyboxLocation = this->shader->getUniformLocation("skybox");
    this->projectionMatrixLocation = this->shader->getUniformLocation("projection");
    this->viewMatrixLocation = this->shader->getUniformLocation("viewMatrix");
    return true;
  }

  Int32 BasicCubeMaterial::getShaderLocation(StandardAttribute attribute) {
    switch(attribute) {
      case StandardAttribute::Position:
        return this->positionLocation;
      case StandardAttribute::Color:
        return this->colorLocation;
      default:
        return -1;
    }
  }

  Int32 BasicCubeMaterial::getShaderLocation(StandardUniform uniform) {
    switch(uniform) {
      case StandardUniform::ProjectionMatrix:
        return this->projectionMatrixLocation;
      case StandardUniform::ViewMatrix:
        return this->viewMatrixLocation;
      default:
        return -1;
    }
  }

  void BasicCubeMaterial::setSkyboxTexture(std::shared_ptr<CubeTexture> texture) {
    this->skyboxTexture = texture;
  }

  void BasicCubeMaterial::sendCustomUniformsToShader() {
    this->shader->setTextureCube(0, this->skyboxTexture->getTextureID());
    this->shader->setUniform1i(skyboxLocation, 0);
  }
}
