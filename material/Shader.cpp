#include "Shader.h"
#include "../common/debug.h"
#include <stdlib.h>
#include <string.h>

namespace Core {

  Shader::Shader() {

  }

  Shader::~Shader() {
    
  }

  Shader::Shader(const std::string& vertex, const std::string& fragment): ready(false) {
    this->vertexSource = vertex;
    this->fragmentSource = fragment;
  }

  Shader::Shader(const char vertex[], const char fragment[]): ready(false) {
    this->vertexSource = vertex;
    this->fragmentSource = fragment;
  }

  Bool Shader::isReady() const {
    return this->ready;
  }

}