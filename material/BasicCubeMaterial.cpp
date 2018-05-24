#include "BasicCubeMaterial.h"
#include "StandardAttributes.h"
#include "StandardUniforms.h"

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

  BasicCubeMaterial::BasicCubeMaterial() {

  }

  Bool BasicCubeMaterial::build() {
    std::string vertexSrc = vertexShader;
    std::string fragmentSrc = fragmentShader;
    Bool ready = this->buildFromSource(vertexSrc, fragmentSrc);
    if (!ready) {
      return false;
    }
    this->positionLocation = glGetAttribLocation(this->shader->getProgram(), "pos");
    this->colorLocation = glGetAttribLocation(this->shader->getProgram(), "color");
    this->skyboxLocation = glGetAttribLocation(this->shader->getProgram(), "skybox");
    this->projectionMatrixLocation = glGetUniformLocation(this->shader->getProgram(), "projection");
    this->viewMatrixLocation = glGetUniformLocation(this->shader->getProgram(), "viewMatrix");
    return true;
  }

  GLint BasicCubeMaterial::getShaderLocation(StandardAttributes attribute) {
    switch(attribute) {
      case StandardAttributes::Position:
        return this->positionLocation;
      case StandardAttributes::Color:
        return this->colorLocation;
      default:
        return -1;
    }
  }

  GLint BasicCubeMaterial::getShaderLocation(StandardUniforms uniform) {
    switch(uniform) {
      case StandardUniforms::ProjectionMatrix:
        return this->projectionMatrixLocation;
      case StandardUniforms::ViewMatrix:
        return this->viewMatrixLocation;
      default:
        return -1;
    }
  }

  void BasicCubeMaterial::setSkyboxTexture(std::shared_ptr<CubeTexture> texture) {
    this->skyboxTexture = texture;
  }

  void BasicCubeMaterial::sendCustomUniformsToShader() {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, this->skyboxTexture->getTextureID());
    glUniform1i(skyboxLocation, 0);
  }
}
