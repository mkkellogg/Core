//
// Created by Mark Kellogg on 12/15/17.
//

#include <memory>
#include "../common/debug.h"
#include "Material.h"

namespace MPM {

  Material::Material(): ready(false) {

  }

  Material::Material(std::shared_ptr<Shader> shader) {
    this->setShader(shader);
    this->ready = this->shader && this->shader->isReady();
  }

  void Material::setShader(std::shared_ptr<Shader> shader) {
    this->shader = shader;
  }

  std::shared_ptr<Shader> Material::getShader() {
    return this->shader;
  }

  Bool Material::buildFromSource(const std::string& vertexSource, const std::string& fragmentSource) {
    std::shared_ptr<Shader> shader = std::make_shared<Shader>(vertexSource, fragmentSource);
    Bool success = shader->build();
    if(!success) {
      this->ready = false;
      return false;
    }
   this->setShader(shader);
   return true;
  }

}